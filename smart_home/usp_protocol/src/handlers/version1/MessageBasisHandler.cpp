#include "../../../include/handlers/version1/MessageBasisHandler.h"


namespace smart_home::usp_protocol::handlers::version1 {

    MessageType ProtocolMessageBasisHandler::determineMessageType(const char* buffer, size_t length) const {
        return static_cast<MessageType>(
            buffer[getCommonMessageIndex(CommonMessageIndexes::TYPE_BYTE)]
        );
    }

    uint16_t ProtocolMessageBasisHandler::determineSessionId(const char* buffer, size_t length) const {
        const size_t startByte = getCommonMessageIndex(CommonMessageIndexes::SESSION_ID_START);
        const size_t endByte = getCommonMessageIndex(CommonMessageIndexes::SESSION_ID_END);

        std::string sessionId;
        for (size_t i = startByte; i <= endByte; ++i) {
            if (buffer[i] == '\0') {
                throw std::runtime_error("Session ID is shorter than expected.");
            }
            sessionId += buffer[i];
        }

        return *reinterpret_cast<uint16_t*>(sessionId.data());
    }

    uint16_t ProtocolMessageBasisHandler::determineRequestId(const char* buffer, size_t length) const {
        const size_t startByte = getCommonMessageIndex(CommonMessageIndexes::REQUEST_ID_START);
        const size_t endByte = getCommonMessageIndex(CommonMessageIndexes::REQUEST_ID_END);

        std::string requestId;
        for (size_t i = startByte; i <= endByte; ++i) {
            if (buffer[i] == '\0') {
                throw std::runtime_error("Request ID is shorter than expected.");
            }
            requestId += buffer[i];
        }

        return *reinterpret_cast<uint16_t*>(requestId.data());
    }

    uint32_t ProtocolMessageBasisHandler::determineTimestamp(const char* buffer, size_t length) const {
        const size_t startByte = getCommonMessageIndex(CommonMessageIndexes::TIMESTAMP_START);
        const size_t endByte = getCommonMessageIndex(CommonMessageIndexes::TIMESTAMP_END);

        std::string timestamp;
        for (size_t i = startByte; i <= endByte; ++i) {
            if (buffer[i] == '\0') {
                throw std::runtime_error("Timestamp is shorter than expected.");
            }
            timestamp += buffer[i];
        }

        uint32_t networkOrder;
        std::memcpy(&networkOrder, buffer, sizeof(uint32_t));
        return ntohl(networkOrder);
    }

    CommonMessageData ProtocolMessageBasisHandler::parseCommonData(
        const char* buffer,
        size_t length
    ) const {
        if (length < MessageSettings::MIN_PACKET_SIZE) {
            throw std::runtime_error("Buffer length is less than minimum packet size.");
        }

        return CommonMessageData{
            ProtocolVersion::VERSION_1,
            determineSessionId(buffer, length),
            determineMessageType(buffer, length),
            determineTimestamp(buffer, length),
            determineRequestId(buffer, length)
        };
    };

} // namespace smart_home::usp_protocol::handlers::version1