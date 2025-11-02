#pragma once

#include <smart_home/usp_protocol/include/version1/request/RequestMessage.h>
#include <smart_home/usp_protocol/include/version1/response/ResponseMessage.h>
#include <thread>

#include "../UspServer.h"
#include "../UspServerConfig.h"
#include "./UspServerRequest.h"
#include "./UspServerResponse.h"
#include "./ReferencedCommonData.h"
#include "./packets/PacketPoller.h"


namespace smart_home::usp_server::version1 {

    class UspAsyncServer
        : public UspServer<UspServerRequest, UspServerResponse>
    {
        using ResponseReactionFunction = std::function<void(const UspServerResponse&)>;
        using HandlerFunction = std::function<void(const UspServerRequest&, UspServerResponse&)>;
        using CommonPacketPoller = packets::PacketPoller<
            uint16_t,
            ReferencedCommonData
        >;
    private:
        std::unique_ptr<CommonPacketPoller> commonPacketPoller;
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
            const UspServerClient& client
        ) override;
    };
} // namespace smart_home::usp_server::version1