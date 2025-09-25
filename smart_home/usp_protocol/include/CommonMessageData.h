#pragma once

#include <ctime>

#include "./CommonOptions.h"


namespace smart_home::usp_protocol {

    enum class CommonMessageIndexes : size_t {
        VERSION_BYTE = 0,
        TYPE_BYTE = 1,
        SESSION_ID_START = 2,
        SESSION_ID_END = 3,
        REQUEST_ID_START = 4,
        REQUEST_ID_END = 5,
        TIMESTAMP_START = 6,
        TIMESTAMP_END = 13,
    };

    inline size_t getCommonMessageIndex(const CommonMessageIndexes& index) {
        return static_cast<size_t>(index);
    }

    struct PacketMessage {
    public:
        size_t packetsCount;
        size_t packetIndex;

        PacketMessage(
            const size_t& packetsCount,
            const size_t& packetIndex
        );
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

    struct CommonPacketMessageData
        : public CommonMessageData
        , public PacketMessage
    {
    public:
        CommonPacketMessageData(
            const ProtocolVersion& protocolVersion,
            const uint16_t& sessionId,
            const MessageType& messageType,
            const std::time_t& timestamp,
            const uint16_t& requestId,
            const size_t& packetsCount,
            const size_t& packetIndex
        );
    };
} // namespace smart_home::usp_protocol::messages