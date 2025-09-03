#pragma once

#include "./BinaryMessageHandler.h"
#include "./model/Serialization.h"
#include "./model/Deserialization.h"
#include "../messages/ResponseMessage.h"


namespace smart_home::usp_protocol::handlers {

    using ResponseSerializationResult = model::SerializationResult<
        messages::ResponseMessage,
        messages::ResponseMessageRaw
    >;
    using ResponseDeserializationResult = model::DeserializationResult<
        messages::ResponseMessage,
        messages::ResponseMessageRaw
    >;

    class ResponseMessageHandler final
        : public BinaryMessageHandler<messages::ResponseMessage, messages::RESPONSE_CHUNKS_AMOUNT>
    {
    public:
        std::unique_ptr<ResponseSerializationResult>
            serialize(messages::ResponseMessage*) override;

        std::unique_ptr<ResponseDeserializationResult>
            deserialize(messages::ResponseMessageRaw*) override;
    };
} // namespace smart_home::usp_protocol::handlers