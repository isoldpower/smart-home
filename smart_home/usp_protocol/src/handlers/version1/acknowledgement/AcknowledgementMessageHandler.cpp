#include "../../../../include/handlers/version1/acknowledgement/AcknowledgementMessageHandler.h"


namespace smart_home::usp_protocol::handlers::version1 {

    std::unique_ptr<AcknowledgementSerializationResult>
        AcknowledgementMessageHandler::serialize(AcknowledgementMessage* message)
    {
        // TODO: Implement actual serialization logic
        char resultBuffer[] = "Acknowledgement Example";
        AcknowledgementSerializationResult result {
            true,
            message,
            std::vector(
                resultBuffer,
                resultBuffer + sizeof(resultBuffer) / sizeof(char)
            )
        };

        return std::make_unique<AcknowledgementSerializationResult>(result);
    }

    std::unique_ptr<AcknowledgementDeserializationResult>
        AcknowledgementMessageHandler::deserialize(std::vector<char>* buffer)
    {
        // TODO: Implement actual deserialization logic
        AcknowledgementMessage resultMessage {
            ProtocolVersion::VERSION_1,
            1234,
            time(nullptr),
            3323,
            AcknowledgementStatus::ACKNOWLEDGEMENT_SUCCESS,
            buffer->size(),
            buffer->data()
        };
        model::DeserializationResult result { true, buffer, resultMessage };

        return std::make_unique<AcknowledgementDeserializationResult>(result);
    }
} // namespace smart_home::usp_protocol::handlers::version1