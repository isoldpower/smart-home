#pragma once

#include "../model/ProtocolBasisHandler.h"
#include "CommonMessageData.h"

namespace smart_home::usp_protocol::version1 {

    class MessageBasisHandler : public model::ProtocolBasisHandler {
    private:
        MessageType determineMessageType(const char* buffer, size_t length) const;
        uint16_t determineSessionId(const char* buffer, size_t length = sizeof(uint16_t)) const;
        uint16_t determineRequestId(const char* buffer, size_t length = sizeof(uint16_t)) const;
        uint64_t determineTimestamp(const char* buffer, size_t length = sizeof(uint64_t)) const;

        void appendSessionId(char* buffer, uint16_t sessionId) const;
        void appendRequestId(char* buffer, uint16_t requestId) const;
        void appendTimestamp(char* buffer, uint64_t timestamp) const;
        void appendVersion(char* buffer, uint8_t version) const;
        void appendMessageType(char* buffer, MessageType type) const;
    public:
        ~MessageBasisHandler() override = default;

        CommonMessageData parseCommonData(
            const char* buffer,
            size_t length
        ) const override;

        std::unique_ptr<char[]> serializeCommonData(
            const CommonMessageData& data,
            size_t& outSize
        ) const override;
    };

} // namespace smart_home::usp_protocol::version1