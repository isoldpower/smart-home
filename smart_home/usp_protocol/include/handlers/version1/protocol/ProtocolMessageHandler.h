#pragma once

#include "../../model/Deserialization.h"
#include "../../model/Serialization.h"
#include "../BinaryMessageHandler.h"
#include "./ProtocolMessage.h"


namespace smart_home::usp_protocol::handlers::version1 {

    using ProtocolSerializationResult = model::SerializationResult<
        ProtocolMessage,
        ProtocolMessageRaw
    >;
    using ProtocolDeserializationResult = model::DeserializationResult<
        ProtocolMessage,
        ProtocolMessageRaw
    >;

    class ProtocolMessageHandler final
        : public BinaryMessageHandler<ProtocolMessage, PROTOCOL_CHUNKS_AMOUNT>
    {
    public:
        std::unique_ptr<ProtocolSerializationResult>
            serialize(ProtocolMessage*) override;

        std::unique_ptr<ProtocolDeserializationResult>
            deserialize(ProtocolMessageRaw*) override;
    };

} // namespace smart_home::usp_protocol::handlers::version1