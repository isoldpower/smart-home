#pragma once

#include "../CommonMessageData.h"
#include "../model/ProtocolBasisHandler.h"


namespace smart_home::usp_protocol::version1 {

    class MessageBasisHandler : public model::ProtocolBasisHandler {
    private:
        MessageType determineMessageType(const char* buffer, size_t length) const;
        uint16_t determineSessionId(const char* buffer, size_t length = sizeof(uint16_t)) const;
        uint16_t determineRequestId(const char* buffer, size_t length = sizeof(uint16_t)) const;
        uint64_t determineTimestamp(const char* buffer, size_t length = sizeof(uint64_t)) const;
    public:
        ~MessageBasisHandler() override = default;

        CommonMessageData parseCommonData(const char* buffer, size_t length) const override;
    };

} // namespace smart_home::usp_protocol::version1