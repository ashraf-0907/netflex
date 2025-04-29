#include "net-utils.h"

bool NetUtils::toSockAddr(const string& ip, int port, sockaddr_storage& addr) {
    memset(&addr, 0, sizeof(addr));
    if (ip.find(':') != string::npos) {
        sockaddr_in6* addr6 = reinterpret_cast<sockaddr_in6*>(&addr);
        addr6->sin6_family = AF_INET6;
        addr6->sin6_port = htons(port);
        return inet_pton(AF_INET6, ip.c_str(), &addr6->sin6_addr) == 1;
    } else {
        sockaddr_in* addr4 = reinterpret_cast<sockaddr_in*>(&addr);
        addr4->sin_family = AF_INET;
        addr4->sin_port = htons(port);
        return inet_pton(AF_INET, ip.c_str(), &addr4->sin_addr) == 1;
    }
}
