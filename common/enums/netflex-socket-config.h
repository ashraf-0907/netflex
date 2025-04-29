#ifndef NETFLEX_SOCKET_CONFIG_H
#define NETFLEX_SOCKET_CONFIG_H

#include <sys/socket.h>
#include <netinet/in.h> // For AF_INET, SOCK_STREAM, etc.
#include <string>

using namespace std;

/**
 * @file netflex-socket-config.h
 * @brief Defines enums for socket configurations like domain, type, and protocol.
 */

#define BUFFER_SIZE 4096 


enum class NetFlexAddressFamily {
    IPV4 = AF_INET,
    IPV6 = AF_INET6
};

enum class NetFlexSocketType {
    TCP = SOCK_STREAM,
    UDP = SOCK_DGRAM
};

enum class NetFlexProtocol {
    DEFAULT = 0
};

using AddressFamily = NetFlexAddressFamily;
using SocketType = NetFlexSocketType;
using Protocol = NetFlexProtocol;

constexpr in_addr ANY_IPV4 = {INADDR_ANY};
constexpr in6_addr ANY_IPV6 = IN6ADDR_ANY_INIT;

#define IPv4Address struct sockaddr_in
#define IPv6Address struct sockaddr_in6

struct UdpResponse {
    string dataRecieved;
    sockaddr_storage fromAddress;
    int port;
};

#endif
