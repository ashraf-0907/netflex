
#pragma once

#include <string>
#include <vector>
#include <cstring>
#include <arpa/inet.h>
#include <sstream>
#include <iomanip>

using namespace std;

enum class SocketType {
    STREAM = SOCK_STREAM, // TCP
    DATAGRAM = SOCK_DGRAM // UDP
};

enum class Protocol {
    DEFAULT = 0  // Let OS choose protocol
};

enum class AddressFamily {
    IPv4 = AF_INET, 
    IPv6 = AF_INET6
};

class NetUtils {
public:
    static uint32_t hostToNetwork32(uint32_t value) {
        return htonl(value);
    }

    static uint32_t networkToHost32(uint32_t value) {
        return ntohl(value);
    }

    static uint16_t hostToNetwork16(uint16_t value) {
        return htons(value);
    }

    static uint16_t networkToHost16(uint16_t value) {
        return ntohs(value);
    }

    static bool toSockAddr(const string& ip, int port, sockaddr_storage& addr);

    static string hexDump(const vector<char>& data) {
        ostringstream oss;
        for (char c : data) {
            oss << hex << setw(2) << setfill('0') << (static_cast<int>(c) & 0xff) << " ";
        }
        return oss.str();
    }
};