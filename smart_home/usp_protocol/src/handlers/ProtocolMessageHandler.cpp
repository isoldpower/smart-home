#include "../../include/handlers/version1/ProtocolMessageHandler.h"

namespace smart_home::usp_protocol::handlers {

    std::unique_ptr<ProtocolSerializationResult>
        ProtocolMessageHandler::serialize(messages::ProtocolMessage* message)
    {
        // TODO: Implement proper serialization logic
        messages::ProtocolMessageRaw resultBuffer { "Protocol Example" };
        ProtocolSerializationResult result { true, message, resultBuffer };

        return std::make_unique<ProtocolSerializationResult>(result);
    }

    std::unique_ptr<ProtocolDeserializationResult>
        ProtocolMessageHandler::deserialize(messages::ProtocolMessageRaw* buffer)
    {
        // TODO: Implement proper deserialization logic
        messages::ProtocolMessage resultMessage {
            messages::ProtocolVersion::VERSION_1,
            "sessionId",
            time(nullptr),
            "requestId",
            messages::ProtocolAction::ACTION_HANDSHAKE,
            strlen(buffer->at(0)),
            buffer->at(0)
        };
        model::DeserializationResult result { true, buffer, resultMessage };

        return std::make_unique<ProtocolDeserializationResult>(result);
    }
} // namespace smart_home::usp_protocol::handlers