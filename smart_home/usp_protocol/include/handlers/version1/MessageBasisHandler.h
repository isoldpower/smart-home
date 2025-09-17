#pragma once

#include "../CommonMessageData.h"
#include "../model/ProtocolBasisHandler.h"


namespace smart_home::usp_protocol::handlers::version1 {

    class ProtocolMessageBasisHandler : public model::ProtocolBasisHandler {
    private:
        MessageType determineMessageType(const char* buffer, size_t length) const;
        uint16_t determineSessionId(const char* buffer, size_t length) const;
        uint16_t determineRequestId(const char* buffer, size_t length) const;
        uint32_t determineTimestamp(const char* buffer, size_t length) const;
    public:
        ~ProtocolMessageBasisHandler() override = default;

        CommonMessageData parseCommonData(const char* buffer, size_t length) const override;
    };

} // namespace smart_home::usp_protocol::handlers::version1