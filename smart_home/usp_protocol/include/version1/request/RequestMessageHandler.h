#pragma once

#include <vector>

#include "../../model/Deserialization.h"
#include "../../model/Serialization.h"
#include "../BinaryMessageHandler.h"
#include "./RequestMessage.h"


namespace smart_home::usp_protocol::version1 {

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
    private:
        std::string determineAuth(const char* buffer, size_t length) const;
        uint8_t determineGroup(const char* buffer, size_t length) const;
        uint8_t determineAction(const char* buffer, size_t length) const;
        size_t determinePacketsCount(const char* buffer, size_t length) const;
        size_t determinePacketIndex(const char* buffer, size_t length) const;
        size_t determineSize(const char* buffer, size_t length) const;
        char* parseDataPointer(const char* buffer, size_t length) const;
    public:
        std::unique_ptr<RequestSerializationResult>
            serialize(RequestMessage*) override;

        std::unique_ptr<RequestDeserializationResult>
            deserialize(std::vector<char>*) override;
    };
} // namespace smart_home::usp_protocol::version1