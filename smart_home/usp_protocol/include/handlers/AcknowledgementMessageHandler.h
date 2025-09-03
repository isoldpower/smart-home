#pragma once

#include "./BinaryMessageHandler.h"
#include "./model/Deserialization.h"
#include "./model/Serialization.h"
#include "../messages/AcknowledgementMessage.h"


namespace smart_home::usp_protocol::handlers {

    using AcknowledgementSerializationResult = model::SerializationResult<
        messages::AcknowledgementMessage,
        messages::AcknowledgementMessageRaw
    >;
    using AcknowledgementDeserializationResult = model::DeserializationResult<
        messages::AcknowledgementMessage,
        messages::AcknowledgementMessageRaw
    >;

    class AcknowledgementMessageHandler final
        : public BinaryMessageHandler<messages::AcknowledgementMessage, messages::ACKNOWLEDGEMENT_CHUNKS_AMOUNT>
    {
    public:
        std::unique_ptr<AcknowledgementSerializationResult>
            serialize(messages::AcknowledgementMessage*) override;

        std::unique_ptr<AcknowledgementDeserializationResult>
            deserialize(messages::AcknowledgementMessageRaw*) override;
    };
} // namespace smart_home::usp_protocol::handlers