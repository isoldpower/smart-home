#pragma once

#include <any>
#include <iostream>

#include "./UspServerConfig.h"
#include "./net/NetServer.h"
#include "./UspServerRequest.h"
#include "./UspServerResponse.h"
#include "./handlers/MessageHandlerBuilder.h"
#include "./handlers/version1/MessageBasisHandler.h"


namespace smart_home::usp_protocol {

    using HandlerFunction = std::function<void(
        const usp_protocol::UspServerRequest&,
        usp_protocol::UspServerResponse&
    )>;

    class UspServer {
    private:
        net::NetServer netServer;
        HandlerFunction handler;

        void handleRequestReceived(
            const char* buffer,
            const net::NetServerClientInfo& client
        ) {
            std::unique_ptr<handlers::model::ProtocolBasisHandler> messageHandler =
                        handlers::MessageBasisHandlerBuilder::buildFromVersion(buffer);

            if (messageHandler) {
                messages::CommonMessageData commonData = messageHandler->parseCommonData(
                    buffer,
                    client.bytesReceived
                );
                std::cout << "Received message with ID: " << commonData.requestId << std::endl;
            } else {
                std::cerr << "Failed to parse message; skipping..." << std::endl;
            }
        }

        messages::MessageType determineProtocolVersion(const char* buffer, size_t length) const;
    public:
        explicit UspServer(const UspServerConfig& config)
            : netServer({ config.host, config.port })
        {}

        void addMiddleware(const std::any& middleware) {
            std::cerr << "USP Middlewares are not implemented yet." << std::endl;
        }

        void setHandler(const HandlerFunction& handler) {
            this->handler = handler;
        }

        void startServer(const std::function<bool()>& isRunningSignal = []() { return true; }) {
            netServer.establishConnection();

            char* buffer = new char[messages::MessageSettings::MAX_PACKET_SIZE];
            while (isRunningSignal()) {
                net::NetServerClientInfo client = netServer.receiveMessage(
                    messages::MessageSettings::MAX_PACKET_SIZE,
                    buffer,
                    timeval{ 1, 0 }
                );

                if (client.isSuccessful) {
                    handleRequestReceived(buffer, client);
                }
            }

            netServer.closeConnection();
        }
    };
} // namespace smart_home::usp_protocol