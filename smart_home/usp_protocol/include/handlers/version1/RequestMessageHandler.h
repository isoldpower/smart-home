#pragma once

#include "../../messages/RequestMessage.h"
#include "../model/Deserialization.h"
#include "../model/Serialization.h"
#include "BinaryMessageHandler.h"

namespace smart_home::usp_protocol::handlers {

    using RequestSerializationResult = model::SerializationResult<
        messages::RequestMessage,
        messages::RequestMessageRaw
    >;
    using RequestDeserializationResult = model::DeserializationResult<
        messages::RequestMessage,
        messages::RequestMessageRaw
    >;

    class RequestMessageHandler final
        : public BinaryMessageHandler<messages::RequestMessage, messages::REQUEST_CHUNKS_AMOUNT>
    {
    public:
        std::unique_ptr<RequestSerializationResult>
            serialize(messages::RequestMessage*) override;

        std::unique_ptr<RequestDeserializationResult>
            deserialize(messages::RequestMessageRaw*) override;
    };
} // namespace smart_home::usp_protocol::handlers