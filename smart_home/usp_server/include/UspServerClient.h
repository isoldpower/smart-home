#pragma once

#include "NetServerClientInfo.h"

namespace smart_home::usp_server {

    class UspServerClient : public web_server::NetServerClientInfo {
    public:
        bool isAuthenticated;

        explicit UspServerClient(
            const NetServerClientInfo& clientInfo
        );
    };
} // namespace smart_home::usp_server