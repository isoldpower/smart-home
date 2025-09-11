#pragma once

#include "../../model/Deserialization.h"
#include "../../model/Serialization.h"
#include "../BinaryMessageHandler.h"
#include "./RequestMessage.h"


namespace smart_home::usp_protocol::handlers::version1 {

    using RequestSerializationResult = model::SerializationResult<
        RequestMessage,
        RequestMessageRaw
    >;
    using RequestDeserializationResult = model::DeserializationResult<
        RequestMessage,
        RequestMessageRaw
    >;

    class RequestMessageHandler final
        : public BinaryMessageHandler<RequestMessage, REQUEST_CHUNKS_AMOUNT>
    {
    public:
        std::unique_ptr<RequestSerializationResult>
            serialize(RequestMessage*) override;

        std::unique_ptr<RequestDeserializationResult>
            deserialize(RequestMessageRaw*) override;
    };
} // namespace smart_home::usp_protocol::handlers::version1