#include "../../include/handlers/ResponseMessageHandler.h"


namespace smart_home::usp_protocol::handlers {

    std::unique_ptr<ResponseSerializationResult>
        ResponseMessageHandler::serialize(messages::ResponseMessage* message)
    {
        messages::ResponseMessageRaw resultBuffer { "Response Example" };
        ResponseSerializationResult result { true, message, resultBuffer };

        return std::make_unique<ResponseSerializationResult>(result);
    }

    std::unique_ptr<ResponseDeserializationResult>
        ResponseMessageHandler::deserialize(messages::ResponseMessageRaw* buffer)
    {
        messages::ResponseMessage resultMessage { buffer->at(0) };
        model::DeserializationResult result { true, buffer, resultMessage };

        return std::make_unique<ResponseDeserializationResult>(result);
    }
} // namespace smart_home::usp_protocol::handlers