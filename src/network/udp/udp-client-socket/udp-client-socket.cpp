#include "udp-client-socket.h"
#include "../../../../common/logger/logger.h"

UdpClientSocket::UdpClientSocket(AddressFamily domain)
    : NetFlexSocket(domain, SocketType::DATAGRAM, Protocol::DEFAULT)
{
    auto res = createSocket();
    if (!res.ok())
    {
        LOG_ERROR("Failed to create UDP client socket");
    }
}

UdpClientSocket::~UdpClientSocket()
{
    closeSocket();
}

Response<size_t> UdpClientSocket::sendTo(const string &message, const string &ip, int port)
{
    ServerAddressStruct destination;
    if (!NetUtils::toSockAddr(ip, port, destination))
    {
        LOG_ERROR("Invalid destination address");
        return Response<size_t>::error(SocketStatus::INVALID_ARGUMENT);
    }

    return SocketIOHelper::sendFramedTo(this->socketFd, message, &destination);
}

Response<pair<string, ServerAddressStruct>> UdpClientSocket::receiveFrom()
{
    return SocketIOHelper::recvFramedFrom(this->getSocketFd());
}
