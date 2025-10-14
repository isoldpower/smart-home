#pragma once

#include <thread>

#include "../UspServer.h"
#include "../UspServerConfig.h"
#include "./UspServerRequest.h"
#include "./UspServerResponse.h"


namespace smart_home::usp_server::version1 {

    class UspAsyncServer
        : public UspServer<UspServerRequest, UspServerResponse>
    {
        using ResponseReactionFunction = std::function<void(const UspServerResponse&)>;
        using HandlerFunction = std::function<void(const UspServerRequest&, UspServerResponse&)>;
    public:
        explicit UspAsyncServer(
            const UspServerConfig& config,
            const HandlerFunction& onRequest
        );

        void sendRequest(
            const UspServerRequest& request,
            const web_server::NetServerClientInfo& client,
            ResponseReactionFunction onResponse
        ) override;

        void tryReceiveMessage(
            const timeval& timeout
        ) override;

        void proceedMessage(
            const char* buffer,
            size_t length
        ) override;
    };
} // namespace smart_home::usp_server::version1