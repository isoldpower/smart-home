#include "../include/UspServer.h"

#include "../include/handlers/CommonMessageData.h"
#include "../include/handlers/MessageBasisHandlerBuilder.h"
#include "../include/handlers/version1/acknowledgement/AcknowledgementMessage.h"
#include "../include/handlers/version1/acknowledgement/AcknowledgementMessageHandler.h"
#include "../include/handlers/version1/request/RequestMessageHandler.h"
#include "smart_home/usp_protocol/include/handlers/MessageUnion.h"

namespace smart_home::usp_protocol {

    UspServer::UspServer(const UspServerConfig& config)
        : netServer(web_server::NetServerConfig{ config.host, config.port })
        , requestPacketPoller(config.requestPacketPoller)
    {}

    UspServer::~UspServer() {
        delete requestPacketPoller;
    }

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
        std::unique_ptr<handlers::model::ProtocolBasisHandler> commonMessageHandler =
                    handlers::MessageBasisHandlerBuilder::buildFromVersion(buffer);

        if (!commonMessageHandler) {
            std::cerr << "Failed to identify protocol version; skipping..." << std::endl;
            return;
        }

        const handlers::CommonMessageData commonData = commonMessageHandler->parseCommonData(
            buffer,
            client.bytesReceived
        );
        const handlers::MessageUnion messageUnion(commonData, buffer, client.bytesReceived);
        if (!messageUnion.constructedSuccessfully) {
            std::cerr << "Failed to parse request message; skipping..." << std::endl;
            return;
        }

        switch (messageUnion.type) {
            case handlers::MessageType::MESSAGE_REQUEST:
                std::cout << "Received REQUEST message (ID: "
                    << messageUnion.messageVariants.requestMessage->requestId
                    << ")" << std::endl;
                // TODO: process request message
                break;
            case handlers::MessageType::MESSAGE_RESPONSE:
                std::cout << "Received RESPONSE message (ID: "
                    << messageUnion.messageVariants.responseMessage->requestId
                    << ")" << std::endl;
                // TODO: process response message
                break;
            case handlers::MessageType::MESSAGE_ACKNOWLEDGEMENT:
                std::cout << "Received ACK message (ID: "
                    << messageUnion.messageVariants.acknowledgementMessage->requestId
                    << ")" << std::endl;
                // TODO: process acknowledgement message
                break;
            case handlers::MessageType::MESSAGE_PROTOCOL:
                std::cout << "Received PROTO message (ID: "
                    << messageUnion.messageVariants.protocolMessage->requestId
                    << ")" << std::endl;
                // TODO: process protocol message
                break;
            default:
                std::cerr << "Unknown message type received; skipping..." << std::endl;
                return;
        }

        // switch (commonData.type) {
        //     case handlers::MessageType::MESSAGE_REQUEST: {
        //         RequestMessageHandler requestHandler{};
        //         std::unique_ptr<RequestDeserializationResult> deserialized = requestHandler.deserialize(&bufferVector);
        //
        //         if (sendAcknowledgement(commonData, client, deserialized->isSuccess()) == false) {
        //             std::cerr << "Skipping further processing due to ACK failure." << std::endl;
        //             return;
        //         } else if (deserialized->isSuccess() == false) {
        //             std::cerr << "Received invalid request message; skipping..." << std::endl;
        //             return;
        //         }
        //
        //         int addResult = requestPacketPoller->addPacket(
        //             commonData.requestId,
        //             deserialized->getDeserializationState()
        //         );
        //         if (addResult < 0) {
        //             std::cerr << "Failed to add packet to poller; skipping..." << std::endl;
        //             return;
        //         } else if (addResult == 0) {
        //             std::cout << "Packet added to poller; waiting for more packets..." << std::endl;
        //             return;
        //         }
        //         std::vector<RequestMessage*> allPackets = requestPacketPoller->getAllPackets(commonData.requestId);
        //         if (allPackets.empty()) {
        //             std::cerr << "Failed to reassemble full request; skipping..." << std::endl;
        //             return;
        //         }
        //
        //         RequestMessage* referencePacket = allPackets.front();
        //         std::string fullPayload;
        //         for (const auto& packet : allPackets) {
        //             fullPayload += packet->data;
        //             delete packet;
        //         }
        //         UspServerRequest request{
        //             referencePacket->protocolVersion,
        //             referencePacket->sessionId,
        //             referencePacket->type,
        //             referencePacket->timestamp,
        //             referencePacket->requestId,
        //             referencePacket->auth,
        //             referencePacket->actionGroup,
        //             referencePacket->action,
        //             fullPayload.size(),
        //             fullPayload
        //         };
        //
        //         std::cout << "Reassembled full request: " << request << std::endl;
        //         // TODO:
        //         // 1) Execute middlewares
        //         // 2) Call the user-defined handler
        //         // 3) Send response back using sendResponse()
        //         break;
        //     }
        //     case handlers::MessageType::MESSAGE_ACKNOWLEDGEMENT: {
        //         AcknowledgementMessageHandler acknowledgementHandler;
        //         std::unique_ptr<AcknowledgementDeserializationResult> deserialized = acknowledgementHandler.deserialize(&bufferVector);
        //
        //         if (deserialized->isSuccess() == false) {
        //             std::cerr << "Received invalid acknowledgement message; skipping..." << std::endl;
        //             return;
        //         }
        //
        //         std::cout << "Received ACK for request ID: "
        //                   << deserialized->getDeserializationState()->requestId
        //                   << " with status: "
        //                   << static_cast<int>(deserialized->getDeserializationState()->status)
        //                   << std::endl;
        //         std::string requestId = deserialized->getDeserializationState()->requestId;
        //         if (pendingRequests.contains(requestId)) {
        //             delete pendingRequests[deserialized->getDeserializationState()->requestId];
        //             pendingRequests.erase(deserialized->getDeserializationState()->requestId);
        //         } else if (pendingResponses.contains(requestId)) {
        //             delete pendingResponses[deserialized->getDeserializationState()->requestId];
        //             pendingResponses.erase(deserialized->getDeserializationState()->requestId);
        //         }
        //
        //         break;
        //     }
        //     case handlers::MessageType::MESSAGE_RESPONSE: {
        //         if (sendAcknowledgement(commonData, client, true) == false) {
        //             std::cerr << "Skipping further processing due to ACK failure." << std::endl;
        //             return;
        //         }
        //
        //         // 2. Stop related timers
        //         // 3. Call related handler
        //         break;
        //     }
        //     case handlers::MessageType::MESSAGE_PROTOCOL: {
        //         // 1. Handle protocol message (e.g., ping, pong)
        //         // 2. Send ACK back if needed
        //         // 3. Call related handler if needed
        //         break;
        //     }
        //     default: {
        //         std::cerr << "Unknown message type received; skipping..." << std::endl;
        //         return;
        //     }
        // }
        //
        // // Scenarios:
        // // 1) Server sending a request: send request, wait for ACK, wait for response, send ACK
        // // 2) Server receiving a request: receive request, send ACK, process request, send response, wait for ACK
        //
        // // TODO: Further processing steps:
        // // 1. Validate message (e.g., checksum, length)
        // // 2 (WIP). Packet polling (push to poll, if not last packet - end)
        // // 3. Reassemble full message from packets
        // //
        // // 4. Start related timers
        // // 5. Deserialize to message
        // // 6. Execute middlewares
        // // 7. Call the user-defined handler
        // std::cout << "Received message with ID: " << commonData.requestId << std::endl;
    }

    bool UspServer::sendAcknowledgement(
        const handlers::CommonMessageData& commonData,
        const web_server::NetServerClientInfo& client,
        bool isSuccess
    ) const {
        AcknowledgementMessage acknowledgementMessage {
            handlers::ProtocolVersion::VERSION_1,
            commonData.sessionId,
            std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
            commonData.requestId,
            isSuccess
                ? AcknowledgementStatus::ACKNOWLEDGEMENT_SUCCESS
                : AcknowledgementStatus::ACKNOWLEDGEMENT_ERROR_NOACK
        };

        AcknowledgementMessageHandler messageHandler;
        std::unique_ptr<AcknowledgementSerializationResult> serializationResult = messageHandler.serialize(&acknowledgementMessage);
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
} // namespace smart_home::usp_protocol