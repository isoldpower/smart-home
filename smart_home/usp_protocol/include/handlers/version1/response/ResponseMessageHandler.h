#pragma once

#include <vector>

#include "../../model/Deserialization.h"
#include "../../model/Serialization.h"
#include "../BinaryMessageHandler.h"
#include "./ResponseMessage.h"


namespace smart_home::usp_protocol::handlers::version1 {

    using ResponseSerializationResult = model::SerializationResult<
        ResponseMessage,
        std::vector<char>
    >;
    using ResponseDeserializationResult = model::DeserializationResult<
        ResponseMessage,
        std::vector<char>
    >;

    class ResponseMessageHandler final
        : public BinaryMessageHandler<ResponseMessage>
    {
    public:
        std::unique_ptr<ResponseSerializationResult>
            serialize(ResponseMessage*) override;

        std::unique_ptr<ResponseDeserializationResult>
            deserialize(std::vector<char>*) override;
    };
} // namespace smart_home::usp_protocol::handlers::version1