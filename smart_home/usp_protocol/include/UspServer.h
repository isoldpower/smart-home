#pragma once

#include <smart_home/web_server/include/NetServer.h>

#include <any>
#include <iostream>

#include "./UspServerConfig.h"
#include "./UspServerRequest.h"
#include "./UspServerResponse.h"


namespace smart_home::usp_protocol {

    using HandlerFunction = std::function<void(
        const usp_protocol::UspServerRequest&,
        usp_protocol::UspServerResponse&
    )>;

    class UspServer {
    private:
        web_server::NetServer netServer;
        HandlerFunction handler;

        void handleRequestReceived(
            const char* buffer,
            const web_server::NetServerClientInfo& client
        );
    public:
        explicit UspServer(const UspServerConfig& config);

        void addMiddleware(const std::any& middleware);
        void setHandler(const HandlerFunction& handler);
        void startServer(const std::function<bool()>& isRunningSignal = []() { return true; });
    };
} // namespace smart_home::usp_protocol