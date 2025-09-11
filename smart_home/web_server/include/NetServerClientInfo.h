#pragma once

#include <netinet/in.h>


namespace smart_home::web_server {

    struct NetServerClientInfo {
    public:
        int socketDescriptor{-1};
        sockaddr_in clientAddress{};
        socklen_t clientLength{sizeof(clientAddress)};
        ssize_t bytesReceived{-1};
        bool isSuccessful{false};

        NetServerClientInfo(
            sockaddr_in clientAddress,
            socklen_t clientLength,
            ssize_t bytesReceived,
            bool isSuccessful
        );
    };
} // namespace smart_home::usp_protocol::net