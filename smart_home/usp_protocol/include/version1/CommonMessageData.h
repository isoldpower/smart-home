#pragma once

#include <ctime>
#include <cstdint>

#include "../CommonOptions.h"


namespace smart_home::usp_protocol::version1 {

    enum class CommonMessageIndexes : size_t {
        VERSION_BYTE = 0,
        TYPE_BYTE = 1,
        SESSION_ID_START = 2,
        SESSION_ID_END = 3,
        REQUEST_ID_START = 4,
        REQUEST_ID_END = 5,
        TIMESTAMP_START = 6,
        TIMESTAMP_END = 13,
        PACKET_INDEX_BYTE = 14,
        PACKETS_COUNT_BYTE = 15,
    };

    consteval size_t getCommonMessageIndex(const CommonMessageIndexes& index) {
        return static_cast<size_t>(index);
    }

    enum class MessageType : uint8_t {
        MESSAGE_REQUEST = 0x01,
        MESSAGE_RESPONSE = 0x02,
        MESSAGE_ACKNOWLEDGEMENT = 0x03,
        MESSAGE_PROTOCOL = 0x04,
        MESSAGE_UNKNOWN = 0xFF,
    };

    enum MessageSettings : size_t {
        MAX_PACKET_SIZE = 4096,
        COMMON_PACKET_SIZE = static_cast<size_t>(CommonMessageIndexes::PACKETS_COUNT_BYTE) + 1,
        MIN_PACKET_SIZE = static_cast<size_t>(CommonMessageIndexes::PACKETS_COUNT_BYTE) + 1
    };

    struct CommonMessageData {
    public:
        ProtocolVersion protocolVersion;
        uint16_t sessionId;
        MessageType type;
        uint64_t timestamp;
        uint16_t requestId;

        CommonMessageData(
            const ProtocolVersion& protocolVersion,
            const uint16_t& sessionId,
            const MessageType& messageType,
            const uint64_t& timestamp,
            const uint16_t& requestId
        );
    };

    struct CommonMessagePacketData
        : public CommonMessageData
    {
    public:
        size_t packetIndex;
        size_t packetsCount;

        CommonMessagePacketData(
            const ProtocolVersion& protocolVersion,
            const uint16_t& sessionId,
            const MessageType& messageType,
            const uint64_t& timestamp,
            const uint16_t& requestId,
            size_t packetIndex,
            size_t packetsCount
        );
    };


} // namespace smart_home::usp_protocol::version1