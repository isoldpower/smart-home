#pragma once
#include <netinet/in.h>

namespace smart_home::usp_protocol::server {

    struct UspServerClientInfo {
        int socketDescriptor{-1};
        sockaddr_in clientAddress{};
        socklen_t clientLength{sizeof(clientAddress)};

        UspServerClientInfo(const sockaddr_in clientAddress, const socklen_t clientLength)
            : clientAddress(clientAddress)
            , clientLength(clientLength)
        {}
    };
} // namespace smart_home::usp_protocol::server