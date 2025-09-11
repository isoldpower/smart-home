#include "../../include/handlers/version1/ResponseMessageHandler.h"

namespace smart_home::usp_protocol::handlers {

    std::unique_ptr<ResponseSerializationResult>
        ResponseMessageHandler::serialize(messages::ResponseMessage* message)
    {
        // TODO: Implement proper serialization logic
        messages::ResponseMessageRaw resultBuffer { "Response Example" };
        ResponseSerializationResult result { true, message, resultBuffer };

        return std::make_unique<ResponseSerializationResult>(result);
    }

    std::unique_ptr<ResponseDeserializationResult>
        ResponseMessageHandler::deserialize(messages::ResponseMessageRaw* buffer)
    {
        // TODO: Implement proper deserialization logic
        messages::ResponseMessage resultMessage {
            messages::ProtocolVersion::VERSION_1,
            "sessionId",
            time(nullptr),
            "requestId",
            1,
            0,
            messages::ResponseStatus::STATUS_OK,
            strlen(buffer->at(0)),
            buffer->at(0)
        };
        model::DeserializationResult result { true, buffer, resultMessage };

        return std::make_unique<ResponseDeserializationResult>(result);
    }
} // namespace smart_home::usp_protocol::handlers