#include "../../../../include/handlers/version1/acknowledgement/AcknowledgementMessageHandler.h"


namespace smart_home::usp_protocol::handlers::version1 {

    std::unique_ptr<AcknowledgementSerializationResult>
        AcknowledgementMessageHandler::serialize(AcknowledgementMessage* message)
    {
        // TODO: Implement actual serialization logic
        AcknowledgementMessageRaw resultBuffer { "Acknowledgement Example" };
        AcknowledgementSerializationResult result { true, message, resultBuffer };

        return std::make_unique<AcknowledgementSerializationResult>(result);
    }

    std::unique_ptr<AcknowledgementDeserializationResult>
        AcknowledgementMessageHandler::deserialize(AcknowledgementMessageRaw* buffer)
    {
        // TODO: Implement actual deserialization logic
        AcknowledgementMessage resultMessage {
            ProtocolVersion::VERSION_1,
            "sessionId",
            time(nullptr),
            "requestId",
            AcknowledgementStatus::ACKNOWLEDGEMENT_SUCCESS,
            strlen(buffer->at(0)),
            buffer->at(0)
        };
        model::DeserializationResult result { true, buffer, resultMessage };

        return std::make_unique<AcknowledgementDeserializationResult>(result);
    }
} // namespace smart_home::usp_protocol::handlers::version1