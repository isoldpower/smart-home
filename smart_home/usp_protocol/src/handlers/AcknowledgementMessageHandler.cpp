#include "../../include/handlers/AcknowledgementMessageHandler.h"


namespace smart_home::usp_protocol::handlers {

    std::unique_ptr<AcknowledgementSerializationResult>
        AcknowledgementMessageHandler::serialize(messages::AcknowledgementMessage* message)
    {
        messages::AcknowledgementMessageRaw resultBuffer { "Acknowledgement Example" };
        AcknowledgementSerializationResult result { true, message, resultBuffer };

        return std::make_unique<AcknowledgementSerializationResult>(result);
    }

    std::unique_ptr<AcknowledgementDeserializationResult>
        AcknowledgementMessageHandler::deserialize(messages::AcknowledgementMessageRaw* buffer)
    {
        messages::AcknowledgementMessage resultMessage { buffer->at(0) };
        model::DeserializationResult result { true, buffer, resultMessage };

        return std::make_unique<AcknowledgementDeserializationResult>(result);
    }
} // namespace smart_home::usp_protocol::handlers