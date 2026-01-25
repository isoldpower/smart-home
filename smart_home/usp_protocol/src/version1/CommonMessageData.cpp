#include "../../include/version1/CommonMessageData.h"


namespace smart_home::usp_protocol::version1 {

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

    CommonMessagePacketData::CommonMessagePacketData(
        const ProtocolVersion& protocolVersion,
        const uint16_t& sessionId,
        const MessageType& messageType,
        const uint64_t& timestamp,
        const uint16_t& requestId,
        const size_t packetIndex,
        const size_t packetsCount
    )
        : CommonMessageData(
            protocolVersion,
            sessionId,
            messageType,
            timestamp,
            requestId
        )
        , packetIndex(packetIndex)
        , packetsCount(packetsCount)
    {}

} // namespace smart_home::usp_protocol::version1