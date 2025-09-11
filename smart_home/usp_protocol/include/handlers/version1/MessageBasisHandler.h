#pragma once

#include "../model/ProtocolBasisHandler.h"


namespace smart_home::usp_protocol::handlers::version1 {

    class ProtocolMessageBasisHandler : public model::ProtocolBasisHandler {
    private:
        messages::MessageType determineMessageType(const char* buffer, size_t length) const {
            // Parse logic
            return messages::MessageType::MESSAGE_UNKNOWN;
        }

        std::string determineSessionId(const char* buffer, size_t length) const {
            // Parse logic
            return "default-session-id";
        }

        std::string determineRequestId(const char* buffer, size_t length) const {
            // Parse logic
            return "default-request-id";
        }

        time_t determineTimestamp(const char* buffer, size_t length) const {
            // Parse logic
            return std::time(nullptr);
        }
    public:
        ~ProtocolMessageBasisHandler() override = default;
        messages::CommonMessageData parseCommonData(const char* buffer, size_t length) const override {
            return messages::CommonMessageData{
                messages::ProtocolVersion::VERSION_1,
                determineSessionId(buffer, length),
                determineMessageType(buffer, length),
                determineTimestamp(buffer, length),
                determineRequestId(buffer, length)
            };
        };
    };

} // namespace smart_home::usp_protocol::handlers::model