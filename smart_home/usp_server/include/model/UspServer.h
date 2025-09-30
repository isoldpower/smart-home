#pragma once

#include <smart_home/usp_protocol/include/version1/request/RequestMessage.h>
#include <smart_home/web_server/include/NetServer.h>
#include <any>

#include "../UspServerConfig.h"
#include "../UspServerRequest.h"
#include "../UspServerResponse.h"


namespace smart_home::usp_server {
    using namespace smart_home::usp_protocol;

    using HandlerFunction = std::function<void(
        const UspServerRequest&,
        UspServerResponse&
    )>;

    class UspServer {
    protected:
        web_server::NetServer netServer;
        HandlerFunction handler;
        packets::PacketPoller<version1::RequestMessage> *requestPacketPoller;

        void handleRequestReceived(
            const char*,
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

        void addMiddleware(const std::any& middleware);
        void setHandler(const HandlerFunction& handler);
    };
} // namespace smart_home::usp_server