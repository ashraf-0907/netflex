#pragma once

#include <string>
#include <vector>
#include <cstring>
#include <arpa/inet.h>
#include "../utils/net-utils/net-utils.h"

using namespace std;

class MessageFormatter {
    public:

        static vector<char> encode(const string& message) {
            uint32_t len = NetUtils::hostToNetwork32(static_cast<uint32_t>(message.size()));
            vector<char> packet(4 + message.size());
            memcpy(packet.data(), &len, 4);
            memcpy(packet.data() + 4, message.data(), message.size());
            return packet;
        }
    
        static bool decode(int socketFd, string& outMessage) {
            char lenBuf[4];
            int received = recv(socketFd, lenBuf, 4, MSG_WAITALL);
            if (received != 4) return false;
    
            uint32_t messageLen;
            memcpy(&messageLen, lenBuf, 4);
            messageLen = NetUtils::hostToNetwork32(messageLen);
    
            if (messageLen > 65536) return false; // sanity check
    
            vector<char> buffer(messageLen);
            received = recv(socketFd, buffer.data(), messageLen, MSG_WAITALL);
            if (received != static_cast<int>(messageLen)) return false;
    
            outMessage.assign(buffer.begin(), buffer.end());
            return true;
        }
    };
    