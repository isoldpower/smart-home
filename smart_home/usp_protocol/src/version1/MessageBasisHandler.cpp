#include "../../include/version1/MessageBasisHandler.h"

#include <smart_home/utilities/include/BinaryReader.h>
#include <smart_home/utilities/include/exceptions/LevelException.h>

#include <cstring>
#include <iostream>
#include <string>

#include "../../include/exceptions/ExecutionCodes.h"
#include "../../include/exceptions/ProtocolPacketException.h"
#include "../../include/exceptions/ProtocolStructuralException.h"


namespace smart_home::usp_protocol::version1 {

    CommonMessagePacketData MessageBasisHandler::parseCommonData(
        const char* buffer,
        const size_t length
    ) const {
        if (length < MessageSettings::MIN_PACKET_SIZE) {
            throw exceptions::ProtocolPacketException(
                utilities::exceptions::ExceptionLevel::ERROR,
                exceptions::castedExecutionCode(
                    exceptions::ExecutionCodes::RECEIVED_PACKAGE_SIZE_ERROR
                ),
                "Buffer length is less than minimum packet size."
            );
        }

        return CommonMessagePacketData{
            ProtocolVersion::VERSION_1,
            determineSessionId(buffer, length),
            determineMessageType(buffer, length),
            determineTimestamp(buffer, length),
            determineRequestId(buffer, length),
            determinePacketIndex(buffer, length),
            determinePacketsCount(buffer, length)
        };
    }

    std::unique_ptr<char[]> MessageBasisHandler::serializeCommonData(
        const CommonMessagePacketData& data,
        size_t& outSize
    ) const {
        std::string resultBuffer{};
        resultBuffer.reserve(MessageSettings::MAX_PACKET_SIZE);
        resultBuffer.resize(MessageSettings::COMMON_PACKET_SIZE);

        appendTimestamp(resultBuffer.data(), data.timestamp);
        appendRequestId(resultBuffer.data(), data.requestId);
        appendSessionId(resultBuffer.data(), data.sessionId);
        appendMessageType(resultBuffer.data(), data.type);
        appendVersion(resultBuffer.data(), static_cast<uint8_t>(data.protocolVersion));
        appendPacketIndex(resultBuffer.data(), data.packetIndex);
        appendPacketsCount(resultBuffer.data(), data.packetsCount);

        resultBuffer.shrink_to_fit();
        outSize = resultBuffer.size();
        auto result = std::make_unique<char[]>(outSize);
        std::memcpy(result.get(), resultBuffer.data(), resultBuffer.size());
        return result;
    }

    MessageType MessageBasisHandler::determineMessageType(
        const char* buffer,
        const size_t length
    ) const {
        constexpr size_t typeByteIndex = getCommonMessageIndex(CommonMessageIndexes::TYPE_BYTE);

        if (length < typeByteIndex + 1) {
            throw exceptions::ProtocolPacketException(
                utilities::exceptions::ExceptionLevel::ERROR,
                exceptions::castedExecutionCode(
                    exceptions::ExecutionCodes::RECEIVED_PACKAGE_DATA_ERROR
                ),
                "Buffer length is less than expected for Type byte."
            );
        } else {
            uint8_t typeValue = utilities::BigEndianReader::bytesToUint8(&buffer[typeByteIndex]);
            return static_cast<MessageType>(typeValue);
        }
    }

    uint16_t MessageBasisHandler::determineSessionId(
        const char* buffer,
        size_t length
    ) const {
        constexpr size_t startByte = getCommonMessageIndex(CommonMessageIndexes::SESSION_ID_START);
        constexpr size_t endByte = getCommonMessageIndex(CommonMessageIndexes::SESSION_ID_END);

        if constexpr (endByte - startByte + 1 != sizeof(uint16_t)) {
            throw exceptions::ProtocolPacketException(
                utilities::exceptions::ExceptionLevel::ERROR,
                exceptions::castedExecutionCode(
                    exceptions::ExecutionCodes::RECEIVED_PACKAGE_DATA_ERROR
                ),
                "Session ID bytes chunk size mismatch."
            );
        } else if (length <= endByte) {
            throw exceptions::ProtocolPacketException(
                utilities::exceptions::ExceptionLevel::ERROR,
                exceptions::castedExecutionCode(
                    exceptions::ExecutionCodes::RECEIVED_PACKAGE_SIZE_ERROR
                ),
                "Buffer length is less than expected for Session ID byte."
            );
        } else {
            return utilities::BigEndianReader::bytesToUint16(&buffer[startByte]);
        }
    }

    uint16_t MessageBasisHandler::determineRequestId(
        const char* buffer,
        size_t length
    ) const {
        constexpr size_t startByte = getCommonMessageIndex(CommonMessageIndexes::REQUEST_ID_START);
        constexpr size_t endByte = getCommonMessageIndex(CommonMessageIndexes::REQUEST_ID_END);

        if constexpr (endByte - startByte + 1 != sizeof(uint16_t)) {
            throw exceptions::ProtocolPacketException(
                utilities::exceptions::ExceptionLevel::ERROR,
                exceptions::castedExecutionCode(
                    exceptions::ExecutionCodes::RECEIVED_PACKAGE_DATA_ERROR
                ),
                "Request ID bytes chunk size mismatch."
            );
        } else if (length <= endByte) {
            throw exceptions::ProtocolPacketException(
                utilities::exceptions::ExceptionLevel::ERROR,
                exceptions::castedExecutionCode(
                    exceptions::ExecutionCodes::RECEIVED_PACKAGE_SIZE_ERROR
                ),
                "Buffer length is less than expected for Request ID byte."
            );
        } else {
            return utilities::BigEndianReader::bytesToUint16(&buffer[startByte]);
        }
    }

    uint64_t MessageBasisHandler::determineTimestamp(
        const char* buffer,
        size_t length
    ) const {
        constexpr size_t startByte = getCommonMessageIndex(CommonMessageIndexes::TIMESTAMP_START);
        constexpr size_t endByte = getCommonMessageIndex(CommonMessageIndexes::TIMESTAMP_END);

        if constexpr (endByte - startByte + 1 != sizeof(uint64_t)) {
            throw exceptions::ProtocolPacketException(
                utilities::exceptions::ExceptionLevel::ERROR,
                exceptions::castedExecutionCode(
                    exceptions::ExecutionCodes::RECEIVED_PACKAGE_SIZE_ERROR
                ),
                "Timestamp bytes chunk size mismatch."
            );
        } else if (length <= endByte) {
            throw exceptions::ProtocolPacketException(
                utilities::exceptions::ExceptionLevel::ERROR,
                exceptions::castedExecutionCode(
                    exceptions::ExecutionCodes::RECEIVED_PACKAGE_SIZE_ERROR
                ),
                "Buffer length is less than expected for Session ID byte."
            );
        } else {
            return utilities::BigEndianReader::bytesToUint64(&buffer[startByte]);
        }
    }

    size_t MessageBasisHandler::determinePacketIndex(
        const char* buffer,
        const size_t length
    ) const {
        constexpr size_t packetIndex = getCommonMessageIndex(
            CommonMessageIndexes::PACKET_INDEX_BYTE
        );

        if (length < packetIndex + 1) {
            throw exceptions::ProtocolPacketException(
                utilities::exceptions::ExceptionLevel::ERROR,
                exceptions::castedExecutionCode(
                    exceptions::ExecutionCodes::RECEIVED_PACKAGE_DATA_ERROR
                ),
                "Buffer length is less than expected for Packet Index byte."
            );
        } else {
            return utilities::BigEndianReader::bytesToUint8(&buffer[packetIndex]);
        }
    }

    size_t MessageBasisHandler::determinePacketsCount(
        const char* buffer,
        const size_t length
    ) const {
        constexpr size_t packetsCountByte = getCommonMessageIndex(
            CommonMessageIndexes::PACKETS_COUNT_BYTE
        );

        if (length < packetsCountByte + 1) {
            throw exceptions::ProtocolPacketException(
                utilities::exceptions::ExceptionLevel::ERROR,
                exceptions::castedExecutionCode(
                    exceptions::ExecutionCodes::RECEIVED_PACKAGE_DATA_ERROR
                ),
                "Buffer length is less than expected for Packets Count byte."
            );
        } else {
            return utilities::BigEndianReader::bytesToUint8(&buffer[packetsCountByte]);
        }
    }


    void MessageBasisHandler::appendSessionId(char* buffer, const uint16_t sessionId) const {
        constexpr size_t startByte = getCommonMessageIndex(CommonMessageIndexes::SESSION_ID_START);
        constexpr size_t endByte = getCommonMessageIndex(CommonMessageIndexes::SESSION_ID_END);

        if constexpr (endByte - startByte + 1 != sizeof(uint16_t)) {
            throw exceptions::ProtocolStructuralException(
                utilities::exceptions::ExceptionLevel::ERROR,
                exceptions::castedExecutionCode(
                    exceptions::ExecutionCodes::WRONG_VERSION_STRUCTURE
                ),
                "Session ID size mismatch between passed type size and space available in the protocol message structure."
            );
        } else {
            const std::unique_ptr<char[]> sessionSerialized =
                utilities::BigEndianReader::uint16ToBytes(sessionId);

            std::ranges::copy(
                sessionSerialized.get(),
                sessionSerialized.get() + sizeof(uint16_t),
                &buffer[startByte]
            );
        }
    }

    void MessageBasisHandler::appendRequestId(char* buffer, const uint16_t requestId) const {
        constexpr size_t startByte = getCommonMessageIndex(CommonMessageIndexes::REQUEST_ID_START);
        constexpr size_t endByte = getCommonMessageIndex(CommonMessageIndexes::REQUEST_ID_END);

        if constexpr (endByte - startByte + 1 != sizeof(uint16_t)) {
            throw exceptions::ProtocolStructuralException(
                utilities::exceptions::ExceptionLevel::ERROR,
                exceptions::castedExecutionCode(
                    exceptions::ExecutionCodes::WRONG_VERSION_STRUCTURE
                ),
                "Request ID size mismatch between passed type size and space available in the protocol message structure."
            );
        } else {
            const std::unique_ptr<char[]> requestSerialized =
                utilities::BigEndianReader::uint16ToBytes(requestId);

            std::ranges::copy(
                requestSerialized.get(),
                requestSerialized.get() + sizeof(uint16_t),
                &buffer[startByte]
            );
        }
    }

    void MessageBasisHandler::appendTimestamp(char* buffer, const uint64_t timestamp) const {
        constexpr size_t startByte = getCommonMessageIndex(CommonMessageIndexes::TIMESTAMP_START);
        constexpr size_t endByte = getCommonMessageIndex(CommonMessageIndexes::TIMESTAMP_END);

        if constexpr (endByte - startByte + 1 != sizeof(uint64_t)) {
            throw exceptions::ProtocolStructuralException(
                utilities::exceptions::ExceptionLevel::ERROR,
                exceptions::castedExecutionCode(
                    exceptions::ExecutionCodes::WRONG_VERSION_STRUCTURE
                ),
                "Timestamp size mismatch between passed type size and space available in the protocol message structure."
            );
        } else {
            const std::unique_ptr<char[]> timestampSerialized =
                utilities::BigEndianReader::uint64ToBytes(timestamp);

            std::ranges::copy(
                timestampSerialized.get(),
                timestampSerialized.get() + sizeof(uint64_t),
                &buffer[startByte]
            );
        }
    }

    void MessageBasisHandler::appendVersion(char* buffer, const uint8_t version) const {
        constexpr size_t versionByte = getCommonMessageIndex(CommonMessageIndexes::VERSION_BYTE);

        const std::unique_ptr<char[]> versionSerialized =
            utilities::BigEndianReader::uint8ToBytes(version);
        buffer[versionByte] = versionSerialized[0];
    }

    void MessageBasisHandler::appendMessageType(char* buffer, const MessageType type) const {
        constexpr size_t typeByte = getCommonMessageIndex(CommonMessageIndexes::TYPE_BYTE);

        const std::unique_ptr<char[]> typeSerialized =
            utilities::BigEndianReader::uint8ToBytes(static_cast<uint8_t>(type));
        buffer[typeByte] = typeSerialized[0];
    }

    void MessageBasisHandler::appendPacketIndex(char* buffer, size_t packetIndex) const {
        constexpr size_t packetIndexByte = getCommonMessageIndex(CommonMessageIndexes::PACKET_INDEX_BYTE);

        const std::unique_ptr<char[]> packetIndexSerialized =
            utilities::BigEndianReader::uint8ToBytes(static_cast<uint8_t>(packetIndex));

        buffer[packetIndexByte] = packetIndexSerialized[0];
    }

    void MessageBasisHandler::appendPacketsCount(char* buffer, size_t packetsCount) const {
        constexpr size_t packetsCountByte = getCommonMessageIndex(CommonMessageIndexes::PACKETS_COUNT_BYTE);

        const std::unique_ptr<char[]> packetsCountSerialized =
            utilities::BigEndianReader::uint8ToBytes(static_cast<uint8_t>(packetsCount));
        buffer[packetsCountByte] = packetsCountSerialized[0];
    }
} // namespace smart_home::usp_protocol::version1