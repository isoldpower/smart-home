#pragma once
\
#include "./BinaryMessageHandler.h"
#include "./model/Deserialization.h"
#include "./model/Serialization.h"
#include "../messages/ProtocolMessage.h"


namespace smart_home::usp_protocol::handlers {

    using ProtocolSerializationResult = model::SerializationResult<
        messages::ProtocolMessage,
        messages::ProtocolMessageRaw
    >;
    using ProtocolDeserializationResult = model::DeserializationResult<
        messages::ProtocolMessage,
        messages::ProtocolMessageRaw
    >;

    class ProtocolMessageHandler final
        : public BinaryMessageHandler<messages::ProtocolMessage, messages::PROTOCOL_CHUNKS_AMOUNT>
    {
    public:
        std::unique_ptr<ProtocolSerializationResult>
            serialize(messages::ProtocolMessage*) override;

        std::unique_ptr<ProtocolDeserializationResult>
            deserialize(messages::ProtocolMessageRaw*) override;
    };

} // namespace smart_home::usp_protocol::handlers