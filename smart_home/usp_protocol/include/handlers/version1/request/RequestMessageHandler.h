#pragma once

#include <vector>

#include "../../model/Deserialization.h"
#include "../../model/Serialization.h"
#include "../BinaryMessageHandler.h"
#include "./RequestMessage.h"


namespace smart_home::usp_protocol::handlers::version1 {

    using RequestSerializationResult = model::SerializationResult<
        RequestMessage,
        std::vector<char>
    >;
    using RequestDeserializationResult = model::DeserializationResult<
        RequestMessage,
        std::vector<char>
    >;

    class RequestMessageHandler final
        : public BinaryMessageHandler<RequestMessage>
    {
    public:
        std::unique_ptr<RequestSerializationResult>
            serialize(RequestMessage*) override;

        std::unique_ptr<RequestDeserializationResult>
            deserialize(std::vector<char>*) override;
    };
} // namespace smart_home::usp_protocol::handlers::version1