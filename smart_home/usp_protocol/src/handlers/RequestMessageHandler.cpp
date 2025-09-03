#include "../../include/handlers/RequestMessageHandler.h"


namespace smart_home::usp_protocol::handlers {

    std::unique_ptr<RequestSerializationResult>
        RequestMessageHandler::serialize(messages::RequestMessage* message)
    {
        messages::RequestMessageRaw resultBuffer { "Request Example" };
        RequestSerializationResult result { true, message, resultBuffer };

        return std::make_unique<RequestSerializationResult>(result);
    }

    std::unique_ptr<RequestDeserializationResult>
        RequestMessageHandler::deserialize(messages::RequestMessageRaw* buffer)
    {
        messages::RequestMessage resultMessage { buffer->at(0) };
        model::DeserializationResult result { true, buffer, resultMessage };

        return std::make_unique<RequestDeserializationResult>(result);
    }
} // namespace smart_home::usp_protocol::handlers