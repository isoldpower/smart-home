#pragma once

#include <vector>

#include "../../model/Deserialization.h"
#include "../../model/Serialization.h"
#include "../BinaryMessageHandler.h"
#include "./ResponseMessage.h"


namespace smart_home::usp_protocol::version1 {

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
    private:
        size_t determinePacketsCount(const char* buffer, size_t length) const;
        size_t determinePacketIndex(const char* buffer, size_t length) const;
        uint8_t determineStatus(const char* buffer, size_t length) const;
        size_t determineSize(const char* buffer, size_t length) const;
        char* parseDataPointer(const char* buffer, size_t length) const;

        void appendPacketIndex(std::vector<char>* buffer, size_t packetIndex) const;
        void appendPacketsCount(std::vector<char>* buffer, size_t packetsCount) const;
        void appendStatus(std::vector<char>* buffer, const ResponseStatus& status) const;
        void appendSize(std::vector<char>* buffer, size_t size) const;
        void appendData(std::vector<char>* buffer, const std::string& data) const;
    public:
        std::unique_ptr<ResponseSerializationResult>
            serialize(ResponseMessage*) override;

        std::unique_ptr<ResponseDeserializationResult>
            deserialize(std::vector<char>*) override;
    };
} // namespace smart_home::usp_protocol::version1