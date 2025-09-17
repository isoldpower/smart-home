#pragma once

#include <vector>

#include "../../model/Deserialization.h"
#include "../../model/Serialization.h"
#include "../BinaryMessageHandler.h"
#include "./ProtocolMessage.h"


namespace smart_home::usp_protocol::handlers::version1 {

    using ProtocolSerializationResult = model::SerializationResult<
        ProtocolMessage,
        std::vector<char>
    >;
    using ProtocolDeserializationResult = model::DeserializationResult<
        ProtocolMessage,
        std::vector<char>
    >;

    class ProtocolMessageHandler final
        : public BinaryMessageHandler<ProtocolMessage>
    {
    public:
        std::unique_ptr<ProtocolSerializationResult>
            serialize(ProtocolMessage*) override;

        std::unique_ptr<ProtocolDeserializationResult>
            deserialize(std::vector<char>*) override;
    };

} // namespace smart_home::usp_protocol::handlers::version1