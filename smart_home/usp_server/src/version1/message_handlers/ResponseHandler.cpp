#include "../../../include/version1/message_handlers/ResponseHandler.h"


#include <iostream>
#include <smart_home/usp_protocol/include/version1/response/ResponseMessage.h>
#include <smart_home/usp_protocol/include/version1/response/ResponseMessageHandler.h>

#include "../../../include/version1/packets/SequencedPacketPoller.h"


namespace smart_home::usp_server::version1::message_handlers {

    void ResponseHandler::handleMessage(const char *buffer, UspServerClient client) {
        usp_protocol::version1::ResponseMessageHandler handler;
        auto vectorBuffer = std::vector(
            buffer,
            buffer + client.bytesReceived
        );
        const std::unique_ptr<
            usp_protocol::version1::ResponseDeserializationResult
        > deserializationResult = handler.deserialize(&vectorBuffer);

        if (deserializationResult->isSuccess()) {
            const usp_protocol::version1::ResponseMessage* message =
                deserializationResult->getDeserializationState();

            std::cout << "Message response: " << *message << std::endl;
        } else {
            std::cerr << "Error deserializing Response message" << std::endl;
        }
    }
} // namespace smart_home::usp_server::version1::message_handlers
