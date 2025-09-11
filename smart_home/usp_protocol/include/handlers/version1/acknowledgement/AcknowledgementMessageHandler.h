#pragma once

#include "../../model/Deserialization.h"
#include "../../model/Serialization.h"
#include "../BinaryMessageHandler.h"
#include "./AcknowledgementMessage.h"


namespace smart_home::usp_protocol::handlers::version1 {

    using AcknowledgementSerializationResult = model::SerializationResult<
        AcknowledgementMessage,
        AcknowledgementMessageRaw
    >;
    using AcknowledgementDeserializationResult = model::DeserializationResult<
        AcknowledgementMessage,
        AcknowledgementMessageRaw
    >;

    class AcknowledgementMessageHandler final
        : public BinaryMessageHandler<AcknowledgementMessage, ACKNOWLEDGEMENT_CHUNKS_AMOUNT>
    {
    public:
        std::unique_ptr<AcknowledgementSerializationResult>
            serialize(AcknowledgementMessage*) override;

        std::unique_ptr<AcknowledgementDeserializationResult>
            deserialize(AcknowledgementMessageRaw*) override;
    };
} // namespace smart_home::usp_protocol::handlers::version1