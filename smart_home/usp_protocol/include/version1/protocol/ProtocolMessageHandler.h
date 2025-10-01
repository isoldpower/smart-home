#pragma once

#include <vector>

#include "../../model/Deserialization.h"
#include "../../model/Serialization.h"
#include "../BinaryMessageHandler.h"
#include "./ProtocolMessage.h"


namespace smart_home::usp_protocol::version1 {

    using ProtocolSerializationResult = model::SerializationResult<
        ProtocolMessage,
        std::vector<char>
    >;
    using ProtocolDeserializationResult = model::DeserializationResult<
        ProtocolMessage,
        std::vector<char>
    >;

    class ProtocolMessageHandler final
        : public BinaryMessageHandler<ProtocolMessage>
    {
    private:
        uint8_t determineAction(const char* buffer, size_t length) const;
        size_t determineSize(const char* buffer, size_t length) const;
        char* parseDataPointer(const char* buffer, size_t length) const;

        void appendAction(std::vector<char>* buffer, ProtocolAction action) const;
        void appendSize(std::vector<char>* buffer, uint8_t size) const;
        void appendData(std::vector<char>* buffer, const std::string& data) const;
    public:
        std::unique_ptr<ProtocolSerializationResult>
            serialize(ProtocolMessage*) override;

        std::unique_ptr<ProtocolDeserializationResult>
            deserialize(std::vector<char>*) override;
    };

} // namespace smart_home::usp_protocol::version1