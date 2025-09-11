#pragma once
#include <netinet/in.h>

namespace smart_home::usp_protocol::net {

    struct NetServerClientInfo {
        int socketDescriptor{-1};
        sockaddr_in clientAddress{};
        socklen_t clientLength{sizeof(clientAddress)};
        ssize_t bytesReceived{-1};
        bool isSuccessful{false};

        NetServerClientInfo(
            const sockaddr_in clientAddress,
            const socklen_t clientLength,
            const ssize_t bytesReceived,
            const bool isSuccessful
        )
            : clientAddress(clientAddress)
            , clientLength(clientLength)
            , bytesReceived(bytesReceived)
            , isSuccessful(isSuccessful)
        {}
    };
} // namespace smart_home::usp_protocol::net