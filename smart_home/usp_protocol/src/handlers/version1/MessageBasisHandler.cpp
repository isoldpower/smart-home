#include "../../../include/handlers/version1/MessageBasisHandler.h"


namespace smart_home::usp_protocol::handlers::version1 {

    MessageType ProtocolMessageBasisHandler::determineMessageType(const char* buffer, size_t length) const {
        // Parse logic
        return MessageType::MESSAGE_UNKNOWN;
    }

    std::string ProtocolMessageBasisHandler::determineSessionId(const char* buffer, size_t length) const {
        // Parse logic
        return "default-session-id";
    }

    std::string ProtocolMessageBasisHandler::determineRequestId(const char* buffer, size_t length) const {
        // Parse logic
        return "default-request-id";
    }

    time_t ProtocolMessageBasisHandler::determineTimestamp(const char* buffer, size_t length) const {
        // Parse logic
        return std::time(nullptr);
    }

    CommonMessageData ProtocolMessageBasisHandler::parseCommonData(
        const char* buffer,
        size_t length
    ) const {
        return CommonMessageData{
            ProtocolVersion::VERSION_1,
            determineSessionId(buffer, length),
            determineMessageType(buffer, length),
            determineTimestamp(buffer, length),
            determineRequestId(buffer, length)
        };
    };

} // namespace smart_home::usp_protocol::handlers::version1