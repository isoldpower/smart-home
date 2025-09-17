#pragma once

#include <smart_home/web_server/include/NetServer.h>

#include <any>
#include <iostream>
#include <map>

#include "./UspServerConfig.h"
#include "./UspServerRequest.h"
#include "./UspServerResponse.h"
#include "handlers/PendingRequest.h"

namespace smart_home::usp_protocol {

    using namespace handlers::version1;

    using HandlerFunction = std::function<void(
        const UspServerRequest&,
        UspServerResponse&
    )>;

    class UspServer {
    private:
        web_server::NetServer netServer;
        HandlerFunction handler;
        packets::model::PacketPoller<RequestMessage> *requestPacketPoller;
        std::map<std::string, handlers::PendingRequest<RequestMessage>*> pendingRequests;
        std::map<std::string, handlers::PendingRequest<ResponseMessage>*> pendingResponses;

        void handleRequestReceived(
            const char*,
            const web_server::NetServerClientInfo&
        );
        void sendResponse(
            const UspServerResponse&,
            const web_server::NetServerClientInfo&
        );
        [[nodiscard]] bool sendAcknowledgement(
            const handlers::CommonMessageData&,
            const web_server::NetServerClientInfo&,
            bool
        ) const;
    public:
        explicit UspServer(const UspServerConfig&);
        ~UspServer();

        void addMiddleware(const std::any&);
        void setHandler(const HandlerFunction&);
        void startServer(const std::function<bool()>& = []() { return true; });
    };
} // namespace smart_home::usp_protocol