#include "../../../include/version1/message_handlers/RequestHandler.h"

#include <iostream>
#include <smart_home/usp_protocol/include/version1/request/RequestMessage.h>
#include <smart_home/usp_protocol/include/version1/request/RequestMessageHandler.h>

#include "../../../include/version1/packets/SequencedPacketPoller.h"


namespace smart_home::usp_server::version1::message_handlers {

    RequestHandler::RequestHandler()
        : packetPoller(new packets::SequencedPacketPoller<usp_protocol::version1::RequestMessage>())
    {}

     RequestHandler::~RequestHandler() {
        delete packetPoller;
    }

    void RequestHandler::handleMessage(const char *buffer, UspServerClient client) {
        usp_protocol::version1::RequestMessageHandler handler;
        auto vectorBuffer = std::vector(
            buffer,
            buffer + client.bytesReceived
        );
        const std::unique_ptr<
            usp_protocol::version1::RequestDeserializationResult
        > deserializationResult = handler.deserialize(&vectorBuffer);

        if (deserializationResult->isSuccess()) {
            const usp_protocol::version1::RequestMessage* message =
                deserializationResult->getDeserializationState();

            std::cout << "Message request: " << *message << std::endl;
        } else {
            std::cerr << "Error deserializing Request message" << std::endl;
        }
    }
} // namespace smart_home::usp_server::version1::message_handlers
