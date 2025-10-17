#include "../../../include/version1/message_handlers/AcknowledgementHandler.h"

#include <iostream>
#include <smart_home/usp_protocol/include/version1/acknowledgement/AcknowledgementMessage.h>
#include <smart_home/usp_protocol/include/version1/acknowledgement/AcknowledgementMessageHandler.h>


namespace smart_home::usp_server::version1::message_handlers {

    void AcknowledgementHandler::handleMessage(const char *buffer, UspServerClient client) {
        usp_protocol::version1::AcknowledgementMessageHandler handler;
        auto vectorBuffer = std::vector(
            buffer,
            buffer + client.bytesReceived
        );
        const std::unique_ptr<
            usp_protocol::version1::AcknowledgementDeserializationResult
        > deserializationResult = handler.deserialize(&vectorBuffer);

        if (deserializationResult->isSuccess()) {
            const usp_protocol::version1::AcknowledgementMessage* message =
                deserializationResult->getDeserializationState();

            std::cout << "Message acknowledgement: " << *message << std::endl;
        } else {
            std::cerr << "Error deserializing Acknowledgement message" << std::endl;
        }
    }
} // namespace smart_home::usp_server::version1::message_handlers
