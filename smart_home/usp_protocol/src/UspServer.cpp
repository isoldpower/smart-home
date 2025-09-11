#include "../include/UspServer.h"

#include "../include/handlers/MessageBasisHandlerBuilder.h"
#include "../include/handlers/CommonMessageData.h"


namespace smart_home::usp_protocol {

    UspServer::UspServer(const UspServerConfig& config)
        : netServer(web_server::NetServerConfig{ config.host, config.port })
    {}

    void UspServer::addMiddleware(const std::any& middleware) {
        std::cerr << "USP Middlewares are not implemented yet." << std::endl;
    }

    void UspServer::setHandler(const HandlerFunction& handler) {
        this->handler = handler;
    }

    void UspServer::startServer(const std::function<bool()>& isRunningSignal) {
        netServer.establishConnection();

        char* buffer = new char[handlers::MessageSettings::MAX_PACKET_SIZE];
        while (isRunningSignal()) {
            web_server::NetServerClientInfo client = netServer.receiveMessage(
                handlers::MessageSettings::MAX_PACKET_SIZE,
                buffer,
                timeval{ 1, 0 }
            );

            if (client.isSuccessful) {
                handleRequestReceived(buffer, client);
            }
        }

        netServer.closeConnection();
    }

    void UspServer::handleRequestReceived(
        const char* buffer,
        const web_server::NetServerClientInfo& client
    ) {
        std::unique_ptr<handlers::model::ProtocolBasisHandler> messageHandler =
                    handlers::MessageBasisHandlerBuilder::buildFromVersion(buffer);

        if (messageHandler) {
            handlers::CommonMessageData commonData = messageHandler->parseCommonData(
                buffer,
                client.bytesReceived
            );

            // TODO: Further processing steps:
            // 1. Validate message (e.g., checksum, length)
            // 2. Packet polling (push to poll, if not last packet - end)
            //
            // 3. Reassemble full message from packets
            // 4. Start related timers
            // 5. Deserialize to message
            // 6. Execute middlewares
            // 7. Call the user-defined handler
            std::cout << "Received message with ID: " << commonData.requestId << std::endl;
        } else {
            std::cerr << "Failed to parse message; skipping..." << std::endl;
        }
    }
} // namespace smart_home::usp_protocol