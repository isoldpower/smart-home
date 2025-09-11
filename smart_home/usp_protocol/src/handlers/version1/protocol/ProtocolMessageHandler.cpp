#include "../../../../include/handlers/version1/protocol/ProtocolMessageHandler.h"


namespace smart_home::usp_protocol::handlers::version1 {

    std::unique_ptr<ProtocolSerializationResult>
        ProtocolMessageHandler::serialize(ProtocolMessage* message)
    {
        // TODO: Implement proper serialization logic
        ProtocolMessageRaw resultBuffer { "Protocol Example" };
        ProtocolSerializationResult result { true, message, resultBuffer };

        return std::make_unique<ProtocolSerializationResult>(result);
    }

    std::unique_ptr<ProtocolDeserializationResult>
        ProtocolMessageHandler::deserialize(ProtocolMessageRaw* buffer)
    {
        // TODO: Implement proper deserialization logic
        ProtocolMessage resultMessage {
            ProtocolVersion::VERSION_1,
            "sessionId",
            time(nullptr),
            "requestId",
            ProtocolAction::ACTION_HANDSHAKE,
            strlen(buffer->at(0)),
            buffer->at(0)
        };
        model::DeserializationResult result { true, buffer, resultMessage };

        return std::make_unique<ProtocolDeserializationResult>(result);
    }
} // namespace smart_home::usp_protocol::handlers::version1