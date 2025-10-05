#include "../../include/version1/CommonMessageData.h"


namespace smart_home::usp_protocol {

    PacketMessage::PacketMessage(
        const size_t& packetsCount,
        const size_t& packetIndex
    )
        : packetsCount(packetsCount)
        , packetIndex(packetIndex)
    {}

    CommonMessageData::CommonMessageData(
        const ProtocolVersion& protocolVersion,
        const uint16_t& sessionId,
        const MessageType& messageType,
        const uint64_t& timestamp,
        const uint16_t& requestId
    )
        : protocolVersion(protocolVersion)
        , sessionId(sessionId)
        , type(messageType)
        , timestamp(timestamp)
        , requestId(requestId)
    {}

    CommonPacketMessageData::CommonPacketMessageData(
        const ProtocolVersion& protocolVersion,
        const uint16_t& sessionId,
        const MessageType& messageType,
        const std::time_t& timestamp,
        const uint16_t& requestId,
        const size_t& packetsCount,
        const size_t& packetIndex
    )
        : CommonMessageData(
            protocolVersion,
            sessionId,
            messageType,
            timestamp,
            requestId
        )
        , PacketMessage(
            packetsCount,
            packetIndex
        )
    {}
} // namespace smart_home::usp_protocol