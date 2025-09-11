#include "../../include/handlers/version1/RequestMessageHandler.h"

namespace smart_home::usp_protocol::handlers {

    std::unique_ptr<RequestSerializationResult>
        RequestMessageHandler::serialize(messages::RequestMessage* message)
    {
        // TODO: Implement proper serialization logic
        messages::RequestMessageRaw resultBuffer { "Request Example" };
        RequestSerializationResult result { true, message, resultBuffer };

        return std::make_unique<RequestSerializationResult>(result);
    }

    std::unique_ptr<RequestDeserializationResult>
        RequestMessageHandler::deserialize(messages::RequestMessageRaw* buffer)
    {
        // TODO: Implement proper deserialization logic
        messages::RequestMessage resultMessage {
            messages::ProtocolVersion::VERSION_1,
            "sessionId",
            time(nullptr),
            "requestId",
            1,
            0,
            "",
            0x01,
            0x01,
            strlen(buffer->at(0)),
            buffer->at(0)
        };
        model::DeserializationResult result { true, buffer, resultMessage };

        return std::make_unique<RequestDeserializationResult>(result);
    }
} // namespace smart_home::usp_protocol::handlers