#include "../../../../include/handlers/version1/protocol/ProtocolMessageHandler.h"


namespace smart_home::usp_protocol::handlers::version1 {

    std::unique_ptr<ProtocolSerializationResult>
        ProtocolMessageHandler::serialize(ProtocolMessage* message)
    {
        // TODO: Implement proper serialization logic
        char resultBuffer[] = "Protocol Example";
        ProtocolSerializationResult result {
            true,
            message,
            std::vector(
                resultBuffer,
                resultBuffer + sizeof(resultBuffer) / sizeof(char)
            )
        };

        return std::make_unique<ProtocolSerializationResult>(result);
    }

    std::unique_ptr<ProtocolDeserializationResult>
        ProtocolMessageHandler::deserialize(std::vector<char>* buffer)
    {
        // TODO: Implement proper deserialization logic
        ProtocolMessage resultMessage {
            ProtocolVersion::VERSION_1,
            8823,
            time(nullptr),
            22233,
            ProtocolAction::ACTION_HANDSHAKE,
            buffer->size(),
            buffer->data()
        };
        model::DeserializationResult result { true, buffer, resultMessage };

        return std::make_unique<ProtocolDeserializationResult>(result);
    }
} // namespace smart_home::usp_protocol::handlers::version1