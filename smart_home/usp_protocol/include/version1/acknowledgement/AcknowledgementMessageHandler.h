#pragma once

#include <vector>

#include "../../model/Deserialization.h"
#include "../../model/Serialization.h"
#include "../BinaryMessageHandler.h"
#include "./AcknowledgementMessage.h"


namespace smart_home::usp_protocol::version1 {

    using AcknowledgementSerializationResult = model::SerializationResult<
        AcknowledgementMessage,
        std::vector<char>
    >;
    using AcknowledgementDeserializationResult = model::DeserializationResult<
        AcknowledgementMessage,
        std::vector<char>
    >;

    class AcknowledgementMessageHandler final
        : public BinaryMessageHandler<AcknowledgementMessage>
    {
    private:
        uint8_t determineStatus(const char* buffer, size_t length) const;
        uint8_t determineSize(const char* buffer, size_t length) const;
        char* parseDataPointer(const char* buffer, size_t length) const;

        void appendBasis(std::vector<char>* buffer, const CommonMessageData& data) const;
        void appendStatus(std::vector<char>* buffer, const AcknowledgementStatus& status) const;
        void appendSize(std::vector<char>* buffer, uint8_t size) const;
        void appendData(std::vector<char>* buffer, const std::string& data) const;
    public:
        std::unique_ptr<AcknowledgementSerializationResult>
            serialize(AcknowledgementMessage*) override;

        std::unique_ptr<AcknowledgementDeserializationResult>
            deserialize(std::vector<char>*) override;
    };
} // namespace smart_home::usp_protocol::version1