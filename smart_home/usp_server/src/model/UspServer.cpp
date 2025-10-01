#include "../../include/model/UspServer.h"

#include <smart_home/usp_protocol/include/CommonMessageData.h>
#include <smart_home/usp_protocol/include/MessageUnion.h>
#include <smart_home/usp_protocol/include/MessageHandlerBuilder.h>
#include <iomanip>


namespace smart_home::usp_server {

    UspServer::UspServer(const UspServerConfig& config)
        : netServer(web_server::NetServerConfig{ config.host, config.port })
        , requestPacketPoller(config.requestPacketPoller)
    {}

    UspServer::~UspServer() {
        delete requestPacketPoller;
    }

    void UspServer::addMiddleware(const std::any&) {
        std::cerr << "USP Middlewares are not implemented yet." << std::endl;
    }

    void UspServer::setHandler(const HandlerFunction& handler) {
        this->handler = handler;
    }

    bool UspServer::sendAcknowledgement(
        const CommonMessageData& commonData,
        const web_server::NetServerClientInfo& client,
        bool isSuccess
    ) const {
        version1::AcknowledgementMessage acknowledgementMessage {
            ProtocolVersion::VERSION_1,
            commonData.sessionId,
            0,
            commonData.requestId,
            isSuccess
                ? version1::AcknowledgementStatus::ACKNOWLEDGEMENT_SUCCESS
                : version1::AcknowledgementStatus::ACKNOWLEDGEMENT_ERROR_NOACK
        };

        version1::AcknowledgementMessageHandler messageHandler;
        std::unique_ptr<version1::AcknowledgementSerializationResult> serializationResult
            = messageHandler.serialize(&acknowledgementMessage);
        if (serializationResult->getIsSuccess() == false) {
            std::cerr << "Failed to serialize ACK message." << std::endl;
            return false;
        }

        bool isAcknowledgementSent = netServer.sendMessage(
            serializationResult->getSerializationState()->data(),
            client
        );
        if (isAcknowledgementSent == false) {
            std::cerr << "Failed to send ACK message back to client." << std::endl;
            return false;
        }

        std::cout << "ACK message sent successfully." << std::endl;
        return true;
    }

    void UspServer::handleRequestReceived(
        const char* buffer,
        const web_server::NetServerClientInfo& client
    ) {
        MessageHandlerBuilder basisBuilder{ buffer };
        std::unique_ptr<model::ProtocolBasisHandler> commonMessageHandler =
                    basisBuilder.buildBasisHandler();

        if (!commonMessageHandler) {
            std::cerr << "Failed to identify protocol version; skipping..." << std::endl;
            return;
        }

        const CommonMessageData commonData = commonMessageHandler->parseCommonData(
            buffer,
            client.bytesReceived
        );
        const MessageUnion messageUnion(commonData, buffer, client.bytesReceived);
        if (!messageUnion.constructedSuccessfully) {
            std::cerr << "Failed to parse message union; skipping..." << std::endl;
            return;
        }

        // Scenarios:
        // 1) Server sending a request: send request, wait for ACK, wait for response, send ACK
        // 2) Server receiving a request: receive request, send ACK, process request, send response, wait for ACK

        // TODO: Further processing steps:
        // 1. Validate message (e.g., checksum, length)
        // 2 (WIP). Packet polling (push to poll, if not last packet - end)
        // 3. Reassemble full message from packets
        //
        // 4. Start related timers
        // 5. Deserialize to message
        // 6. Execute middlewares
        // 7. Call the user-defined handler
        switch (messageUnion.type) {
            case MessageType::MESSAGE_REQUEST: {
                std::cout << "Received REQUEST message (ID: "
                    << messageUnion.messageVariants.requestMessage->requestId
                    << ")" << std::endl;
                // TODO: process request message
                break;
            }
            case MessageType::MESSAGE_RESPONSE:
                std::cout << "Received RESPONSE message (ID: "
                    << messageUnion.messageVariants.responseMessage->requestId
                    << ")" << std::endl;
                // TODO: process response message
                break;
            case MessageType::MESSAGE_ACKNOWLEDGEMENT:
                std::cout << "Received ACK message (ID: "
                    << messageUnion.messageVariants.acknowledgementMessage->requestId
                    << ")" << std::endl;
                // TODO: process acknowledgement message
                break;
            case MessageType::MESSAGE_PROTOCOL:
                std::cout << "Received PROTO message (ID: "
                    << messageUnion.messageVariants.protocolMessage->requestId
                    << ")" << std::endl;
                // TODO: process protocol message
                break;
            default:
                std::cerr << "Unknown message type received; skipping..." << std::endl;
                return;
        }
    }
} // namespace smart_home::usp_server