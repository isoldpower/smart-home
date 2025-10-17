#include "../../../include/version1/message_handlers/ProtocolHandler.h"

#include <iostream>
#include <smart_home/usp_protocol/include/version1/protocol/ProtocolMessage.h>
#include <smart_home/usp_protocol/include/version1/protocol/ProtocolMessageHandler.h>



namespace smart_home::usp_server::version1::message_handlers {

    void ProtocolHandler::handleMessage(const char *buffer, UspServerClient client) {
        usp_protocol::version1::ProtocolMessageHandler handler;
        auto vectorBuffer = std::vector(
            buffer,
            buffer + client.bytesReceived
        );
        const std::unique_ptr<
            usp_protocol::version1::ProtocolDeserializationResult
        > deserializationResult = handler.deserialize(&vectorBuffer);

        if (deserializationResult->isSuccess()) {
            const usp_protocol::version1::ProtocolMessage* message =
                deserializationResult->getDeserializationState();

            std::cout << "Message protocol: " << *message << std::endl;
        } else {
            std::cerr << "Error deserializing Protocol message" << std::endl;
        }
    }
} // namespace smart_home::usp_server::version1::message_handlers
