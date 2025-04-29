#pragma once

#include "../utils/net-utils/net-utils.h"
#include "../../wrapper/tcp-server-wrapper.h"
#include "../../wrapper/tcp-client-wrapper.h"
#include "../../wrapper/udp-client-wrapper.h"
#include "../../wrapper/udp-server-wrapper.h"
#include "../../wrapper/ssl-client-wrapper.h"
#include "../../wrapper/ssl-server-wrapper.h"

using namespace std;

class NetflexFactory
{
    static unique_ptr<TcpServerSocketWrapper> createTcpServerSocket(AddressFamily adressFamily);
    static unique_ptr<TcpClientSocketWrapper> createTcpClientSocket(AddressFamily adressFamily,
        const string &serverIp, int serverPort);

    static unique_ptr<UdpServerSocketWrapper> createUdpServer(AddressFamily addressFamily);
    static unique_ptr<UdpClientSocketWrapper> createUdpClient(AddressFamily addressFamily);

    static unique_ptr<SslServerSocketWrapper> createSslServer(AddressFamily addressFamily,
        const string &certPath, const string &keyPath);
    static unique_ptr<SslClientSocketWrapper> createSslClient(AddressFamily addressFamily,
        const string& ip, int port);
};