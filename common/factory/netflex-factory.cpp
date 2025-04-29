#include "./netflex-factory.h"

unique_ptr<TcpClientSocketWrapper> NetflexFactory::createTcpClientSocket(AddressFamily addressFamily,const string &serverIp, int serverPort){
    unique_ptr<TcpClientSocketWrapper> clientSocket =  make_unique<TcpClientSocketWrapper>(addressFamily);
    clientSocket->connectToServer(serverIp, serverPort);
    return clientSocket;
}

unique_ptr<TcpServerSocketWrapper> NetflexFactory::createTcpServerSocket(AddressFamily addressFamily){
    unique_ptr<TcpServerSocketWrapper> serverSocket = make_unique<TcpServerSocketWrapper>(addressFamily);
    return serverSocket;
}

unique_ptr<UdpClientSocketWrapper> NetflexFactory::createUdpClient(AddressFamily addressfamily){
    return make_unique<UdpClientSocketWrapper>(addressfamily);
}

unique_ptr<UdpServerSocketWrapper> NetflexFactory::createUdpServer(AddressFamily addressFamily){
    return make_unique<UdpServerSocketWrapper>(addressFamily);
}

unique_ptr<SslServerSocketWrapper> NetflexFactory::createSslServer(
    AddressFamily addressFamily, const string& certPath, const string& keyPath) {
    return make_unique<SslServerSocketWrapper>(addressFamily, certPath, keyPath);
}

unique_ptr<SslClientSocketWrapper> NetflexFactory::createSslClient(
    AddressFamily addressFamily, const string& ip, int port) {
    return make_unique<SslClientSocketWrapper>(addressFamily, ip, port);
}

