#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

//Structs
#define ServerAddressStruct sockaddr_storage
#define IPv4AddressStruct sockaddr_in
#define IPv6AddressStruct sockaddr_in6
#define SocketStruct sockaddr

//File Descriptors
#define SocketFD int

//Socket Flags
#define SocketBlocking 0
#define SocketNonBlocking SOCK_NONBLOCK

//Defaults
#define DefaultBufferSize 4096
#define MaxPacketSize 65536

//Domain / Type / Protocol (abstracted in enums elsewhere)
#define TCPDomain AF_INET
#define UDPDomain AF_INET
#define IPv6Domain AF_INET6

//Helper alias
using ServerAddress = sockaddr_storage;
using SockAddr = sockaddr;