#include "../../include/handlers/ProtocolMessageHandler.h"


namespace smart_home::usp_protocol::handlers {

    std::unique_ptr<ProtocolSerializationResult>
        ProtocolMessageHandler::serialize(messages::ProtocolMessage* message)
    {
        messages::ProtocolMessageRaw resultBuffer { "Protocol Example" };
        ProtocolSerializationResult result { true, message, resultBuffer };

        return std::make_unique<ProtocolSerializationResult>(result);
    }

    std::unique_ptr<ProtocolDeserializationResult>
        ProtocolMessageHandler::deserialize(messages::ProtocolMessageRaw* buffer)
    {
        messages::ProtocolMessage resultMessage { buffer->at(0) };
        model::DeserializationResult result { true, buffer, resultMessage };

        return std::make_unique<ProtocolDeserializationResult>(result);
    }
} // namespace smart_home::usp_protocol::handlers