#pragma once

#include <vector>

#include "../../model/Deserialization.h"
#include "../../model/Serialization.h"
#include "../BinaryMessageHandler.h"
#include "./AcknowledgementMessage.h"


namespace smart_home::usp_protocol::handlers::version1 {

    using AcknowledgementSerializationResult = model::SerializationResult<
        AcknowledgementMessage,
        std::vector<char>
    >;
    using AcknowledgementDeserializationResult = model::DeserializationResult<
        AcknowledgementMessage,
        std::vector<char>
    >;

    class AcknowledgementMessageHandler final
        : public BinaryMessageHandler<AcknowledgementMessage>
    {
    public:
        std::unique_ptr<AcknowledgementSerializationResult>
            serialize(AcknowledgementMessage*) override;

        std::unique_ptr<AcknowledgementDeserializationResult>
            deserialize(std::vector<char>*) override;
    };
} // namespace smart_home::usp_protocol::handlers::version1