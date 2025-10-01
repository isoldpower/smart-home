#include "../include/NetServerClientInfo.h"


namespace smart_home::web_server {

    NetServerClientInfo::NetServerClientInfo(
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
} // namespace smart_home::web_server