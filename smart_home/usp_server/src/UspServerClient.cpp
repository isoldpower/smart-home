#include "../include/UspServerClient.h"


namespace smart_home::usp_server {

    UspServerClient::UspServerClient(const NetServerClientInfo& clientInfo)
        : NetServerClientInfo(clientInfo)
        , isAuthenticated(false)
    {}
} // namespace smart_home::usp_server