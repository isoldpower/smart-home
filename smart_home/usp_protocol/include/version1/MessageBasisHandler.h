#pragma once

#include "./CommonMessageData.h"
#include "../model/ProtocolBasisHandler.h"


namespace smart_home::usp_protocol::version1 {

    class MessageBasisHandler : public model::ProtocolBasisHandler<CommonMessagePacketData> {
    private:
        MessageType determineMessageType(const char* buffer, size_t length) const;
        uint16_t determineSessionId(const char* buffer, size_t length) const;
        uint16_t determineRequestId(const char* buffer, size_t length) const;
        uint64_t determineTimestamp(const char* buffer, size_t length) const;
        size_t determinePacketIndex(const char* buffer, size_t length) const;
        size_t determinePacketsCount(const char* buffer, size_t length) const;

        void appendSessionId(char* buffer, uint16_t sessionId) const;
        void appendRequestId(char* buffer, uint16_t requestId) const;
        void appendTimestamp(char* buffer, uint64_t timestamp) const;
        void appendVersion(char* buffer, uint8_t version) const;
        void appendMessageType(char* buffer, MessageType type) const;
        void appendPacketIndex(char* buffer, size_t packetIndex) const;
        void appendPacketsCount(char* buffer, size_t packetsCount) const;
    public:
        ~MessageBasisHandler() override = default;

        CommonMessagePacketData parseCommonData(
            const char* buffer,
            size_t length
        ) const override;

        std::unique_ptr<char[]> serializeCommonData(
            const CommonMessagePacketData& data,
            size_t& outSize
        ) const override;
    };

} // namespace smart_home::usp_protocol::version1