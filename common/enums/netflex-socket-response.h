#ifndef NETFLEX_SOCKET_RESPONSE_H
#define NETFLEX_SOCKET_RESPONSE_H

#include <string>
using namespace std; 

/**
 * @file netflex-socket-response.h
 * @brief Defines standard socket responses for NetFlex networking.
 */
enum class NetFlexSocketResponse {
    SUCCESS = 0,
    SOCKET_CREATION_FAILED,
    BIND_FAILED,
    LISTEN_FAILED,
    CONNECT_FAILED,
    SEND_FAILED,
    RECEIVE_FAILED,
    TIMEOUT,
    CONNECTION_CLOSED,
    UNKNOWN_ERROR,
    INVALID_ADDRESS,
};

using Response = NetFlexSocketResponse;

#endif 
