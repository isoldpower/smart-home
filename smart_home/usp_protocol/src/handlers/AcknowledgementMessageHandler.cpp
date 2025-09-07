#include "../../include/handlers/AcknowledgementMessageHandler.h"


namespace smart_home::usp_protocol::handlers {

    std::unique_ptr<AcknowledgementSerializationResult>
        AcknowledgementMessageHandler::serialize(messages::AcknowledgementMessage* message)
    {
        // TODO: Implement actual serialization logic
        messages::AcknowledgementMessageRaw resultBuffer { "Acknowledgement Example" };
        AcknowledgementSerializationResult result { true, message, resultBuffer };

        return std::make_unique<AcknowledgementSerializationResult>(result);
    }

    std::unique_ptr<AcknowledgementDeserializationResult>
        AcknowledgementMessageHandler::deserialize(messages::AcknowledgementMessageRaw* buffer)
    {
        // TODO: Implement actual deserialization logic
        messages::AcknowledgementMessage resultMessage {
            messages::ProtocolVersion::VERSION_1,
            "sessionId",
            time(nullptr),
            "requestId",
            messages::AcknowledgementStatus::ACKNOWLEDGEMENT_SUCCESS,
            strlen(buffer->at(0)),
            buffer->at(0)
        };
        model::DeserializationResult result { true, buffer, resultMessage };

        return std::make_unique<AcknowledgementDeserializationResult>(result);
    }
} // namespace smart_home::usp_protocol::handlers