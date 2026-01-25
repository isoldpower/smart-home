#pragma once

#include <thread>
#include <smart_home/utilities/include/patterns/EventChannel.h>

#include "../UspServer.h"
#include "../UspServerConfig.h"
#include "./UspServerRequest.h"
#include "./UspServerResponse.h"
#include "./ReferencedCommonData.h"
#include "./packets/PacketPoller.h"
#include "./message_handlers/ResponseHandler.h"
#include "./message_handlers/AcknowledgementHandler.h"
#include "./message_handlers/RequestHandler.h"
#include "./message_handlers/ProtocolHandler.h"


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
        std::unique_ptr<message_handlers::ResponseHandler> responseHandler;
        std::unique_ptr<message_handlers::AcknowledgementHandler> acknowledgmentHandler;
        std::unique_ptr<message_handlers::RequestHandler> requestHandler;
        std::unique_ptr<message_handlers::ProtocolHandler> protocolHandler;

        void processCompleteMessage(
            const usp_protocol::version1::CommonMessagePacketData& referencePacket
        ) const;
    public:
        std::shared_ptr<utilities::patterns::EventChannel> eventChannel;

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