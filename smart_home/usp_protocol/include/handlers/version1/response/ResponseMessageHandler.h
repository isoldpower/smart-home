#pragma once

#include "../../model/Deserialization.h"
#include "../../model/Serialization.h"
#include "../BinaryMessageHandler.h"
#include "./ResponseMessage.h"


namespace smart_home::usp_protocol::handlers::version1 {

    using ResponseSerializationResult = model::SerializationResult<version1::ResponseMessage,
        ResponseMessageRaw
    >;
    using ResponseDeserializationResult = model::DeserializationResult<
        ResponseMessage,
        ResponseMessageRaw
    >;

    class ResponseMessageHandler final
        : public BinaryMessageHandler<ResponseMessage, RESPONSE_CHUNKS_AMOUNT>
    {
    public:
        std::unique_ptr<ResponseSerializationResult>
            serialize(ResponseMessage*) override;

        std::unique_ptr<ResponseDeserializationResult>
            deserialize(ResponseMessageRaw*) override;
    };
} // namespace smart_home::usp_protocol::handlers::version1