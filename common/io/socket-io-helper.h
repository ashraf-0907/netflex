#pragma once

#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstring>
#include "../../src/network/ssl/ssl-socket.h"
#include "../response/response.h"
#include "../utils/socket-defines.h"
#include "../response/message-foramtter.h"

class SslSocket;

class SocketIOHelper
{
public:
    // ======= TCP / UDP (raw sockets) =======

    static Response<size_t> sendFramed(int socketFd, const string &message);
    static Response<string> recvFramed(int socketFd);
    static Response<size_t> sendFramedTo(int socketFd, const string &message, const ServerAddressStruct *destinationAddr);
    static Response<pair<string, ServerAddressStruct>> recvFramedFrom(int socketFd);

    // ======= SSL sockets =======

    static Response<size_t> sendFramedSSL(SslSocket *sslSocket, const string &message);
    static Response<string> recvFramedSSL(SslSocket *sslSocket);
};