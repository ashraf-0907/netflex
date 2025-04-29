#include "socket-io-helper.h"

// ----- Plain Sockets -----

Response<size_t> SocketIOHelper::sendFramed(int socketFd, const string &message)
{
    auto framed = MessageFormatter::encode(message);
    size_t totalSent = 0;
    size_t toSend = framed.size();

    while (totalSent < toSend)
    {
        int sent = send(socketFd, framed.data() + totalSent, toSend - totalSent, 0);
        if (sent <= 0)
        {
            return Response<size_t>::error(SocketStatus::WRITE_ERROR, "TCP send failed");
        }
        totalSent += sent;
    }

    return Response<size_t>::success(totalSent);
}

Response<string> SocketIOHelper::recvFramed(int socketFd)
{
    char lenBuf[4];
    int readLen = recv(socketFd, lenBuf, 4, MSG_WAITALL);
    if (readLen != 4)
    {
        return Response<string>::error(SocketStatus::READ_ERROR, "TCP length read failed");
    }

    uint32_t messageLen;
    memcpy(&messageLen, lenBuf, 4);
    messageLen = NetUtils::networkToHost32(messageLen);

    if (messageLen > MaxPacketSize)
    {
        return Response<string>::error(SocketStatus::READ_ERROR, "Message length exceeds buffer limit");
    }

    vector<char> buffer(messageLen);
    int readBody = recv(socketFd, buffer.data(), messageLen, MSG_WAITALL);
    if (readBody != static_cast<int>(messageLen))
    {
        return Response<string>::error(SocketStatus::READ_ERROR, "Incomplete TCP message");
    }

    return Response<string>::success(string(buffer.begin(), buffer.end()));
}

Response<size_t> SocketIOHelper::sendFramedTo(int socketFd, const string &message, const ServerAddressStruct *destinationAddr)
{
    if (!destinationAddr)
    {
        return Response<size_t>::error(SocketStatus::INVALID_ARGUMENT, "Destination address is null");
    }

    auto packet = MessageFormatter::encode(message);
    ssize_t sent = sendto(socketFd, packet.data(), packet.size(), 0, reinterpret_cast<const sockaddr *>(destinationAddr), 
    sizeof(ServerAddressStruct));

    if (sent <= 0)
    {
        return Response<size_t>::error(SocketStatus::WRITE_ERROR, "sendto failed");
    }

    return Response<size_t>::success(static_cast<size_t>(sent));
}

Response<pair<string, ServerAddressStruct>> SocketIOHelper::recvFramedFrom(int socketFd)
{
    char buffer[MaxPacketSize];
    ServerAddressStruct senderAddr;
    socklen_t addrLen = sizeof(senderAddr);

    // Read packet
    ssize_t received = recvfrom(socketFd, buffer, sizeof(buffer), 0,
        reinterpret_cast<sockaddr *>(&senderAddr),&addrLen);

    if (received <= 4)
    {
        return Response<pair<string, ServerAddressStruct>>::error(
            SocketStatus::READ_ERROR, "recvfrom failed or message too short");
    }

    uint32_t len;
    memcpy(&len, buffer, 4);
    len = NetUtils::networkToHost32(len);

    if (len + 4 > static_cast<size_t>(received))
    {
        return Response<pair<string, ServerAddressStruct>>::error(
            SocketStatus::READ_ERROR, "Incomplete message received");
    }

    string message(buffer + 4, len);
    return Response<pair<string, ServerAddressStruct>>::success({message, senderAddr});
}

// ----- SSL Sockets -----

Response<size_t> SocketIOHelper::sendFramedSSL(SslSocket *sslSocket, const string &message)
{
    if (!sslSocket)
        return Response<size_t>::error(SocketStatus::INVALID_ARGUMENT, "Invalid SSL socket");

    auto framed = MessageFormatter::encode(message);
    size_t totalSent = 0;
    size_t toSend = framed.size();

    while (totalSent < toSend)
    {
        int sent = sslSocket->sslWrite(framed.data() + totalSent, toSend - totalSent);
        if (sent <= 0)
        {
            return Response<size_t>::error(SocketStatus::WRITE_ERROR, "SSL send failed");
        }
        totalSent += sent;
    }

    return Response<size_t>::success(totalSent);
}

Response<string> SocketIOHelper::recvFramedSSL(SslSocket *sslSocket)
{
    if (!sslSocket)
        return Response<string>::error(SocketStatus::INVALID_ARGUMENT, "Invalid SSL socket");

    char lenBuf[4];
    int readLen = sslSocket->sslRead(lenBuf, 4);
    if (readLen != 4)
    {
        return Response<string>::error(SocketStatus::READ_ERROR, "SSL length read failed");
    }

    uint32_t messageLen;
    memcpy(&messageLen, lenBuf, 4);
    messageLen = NetUtils::networkToHost32(messageLen);

    if (messageLen > MaxPacketSize)
    {
        return Response<string>::error(SocketStatus::READ_ERROR, "Message length exceeds limit");
    }

    vector<char> buffer(messageLen);
    int readBody = sslSocket->sslRead(buffer.data(), messageLen);
    if (readBody != static_cast<int>(messageLen))
    {
        return Response<string>::error(SocketStatus::READ_ERROR, "Incomplete SSL message");
    }

    return Response<string>::success(string(buffer.begin(), buffer.end()));
}
