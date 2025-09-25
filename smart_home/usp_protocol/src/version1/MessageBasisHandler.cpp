#include "../../include/version1/MessageBasisHandler.h"

#include <iostream>
#include <string>

#include "./BinaryReader.h"

// TODO: Replace all exit() states with exceptions including detailed messages and error codes.
namespace smart_home::usp_protocol::version1 {

    CommonMessageData MessageBasisHandler::parseCommonData(
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
    }

    MessageType MessageBasisHandler::determineMessageType(const char* buffer, size_t length) const {
        const size_t typeByteIndex = getCommonMessageIndex(CommonMessageIndexes::TYPE_BYTE);

        if (length < typeByteIndex + 1) {
            throw std::runtime_error("Buffer length is less than expected for type byte.");
        } else {
            uint8_t typeValue = utilities::BigEndianReader::bytesToUint8(&buffer[typeByteIndex]);
            return static_cast<MessageType>(typeValue);
        }
    }

    uint16_t MessageBasisHandler::determineSessionId(const char* buffer, size_t _) const {
        const size_t startByte = getCommonMessageIndex(CommonMessageIndexes::SESSION_ID_START);
        const size_t endByte = getCommonMessageIndex(CommonMessageIndexes::SESSION_ID_END);

        if (endByte - startByte + 1 != sizeof(uint16_t)) {
            throw std::runtime_error("Session ID size mismatch.");
        } else {
            return utilities::BigEndianReader::bytesToUint16(&buffer[startByte]);
        }
    }

    uint16_t MessageBasisHandler::determineRequestId(const char* buffer, size_t length) const {
        const size_t startByte = getCommonMessageIndex(CommonMessageIndexes::REQUEST_ID_START);
        const size_t endByte = getCommonMessageIndex(CommonMessageIndexes::REQUEST_ID_END);

        if (endByte - startByte + 1 != sizeof(uint16_t)) {
            throw std::runtime_error("Request ID size mismatch.");
        } else {
            return utilities::BigEndianReader::bytesToUint16(&buffer[startByte]);
        }
    }

    uint64_t MessageBasisHandler::determineTimestamp(const char* buffer, size_t length) const {
        const size_t startByte = getCommonMessageIndex(CommonMessageIndexes::TIMESTAMP_START);
        const size_t endByte = getCommonMessageIndex(CommonMessageIndexes::TIMESTAMP_END);

        if (endByte - startByte + 1 != sizeof(uint64_t)) {
            throw std::runtime_error("Timestamp size mismatch.");
        } else {
            return utilities::BigEndianReader::bytesToUint64(&buffer[startByte]);
        }
    }
} // namespace smart_home::usp_protocol::version1