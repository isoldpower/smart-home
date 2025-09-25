#pragma once

#include <smart_home/web_server/include/NetServer.h>
#include <smart_home/usp_protocol/include/version1/request/RequestMessage.h>
#include <any>

#include "./UspServerRequest.h"
#include "./UspServerResponse.h"
#include "./UspServerConfig.h"


namespace smart_home::usp_server {

    using namespace smart_home::usp_protocol;

    using HandlerFunction = std::function<void(
        const UspServerRequest&,
        UspServerResponse&
    )>;

    class UspServer {
    private:
        web_server::NetServer netServer;
        HandlerFunction handler;
        packets::PacketPoller<version1::RequestMessage> *requestPacketPoller;

        void handleRequestReceived(
            const char*,
            const web_server::NetServerClientInfo&
        );
        void sendResponse(
            const UspServerResponse&,
            const web_server::NetServerClientInfo&
        );
        [[nodiscard]] bool sendAcknowledgement(
            const CommonMessageData&,
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
} // namespace smart_home::usp_server