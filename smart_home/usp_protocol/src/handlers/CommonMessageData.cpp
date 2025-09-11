#include "../../include/handlers/CommonMessageData.h"


namespace smart_home::usp_protocol::handlers {

    PacketMessage::PacketMessage(
        const size_t& packetsCount,
        const size_t& packetIndex
    )
        : packetsCount(packetsCount)
        , packetIndex(packetIndex)
    {}

    CommonMessageData::CommonMessageData(
        const ProtocolVersion& protocolVersion,
        std::string sessionId,
        const MessageType& messageType,
        const time_t& timestamp,
        std::string requestId
    )
        : protocolVersion(protocolVersion)
        , sessionId(std::move(sessionId))
        , type(messageType)
        , timestamp(timestamp)
        , requestId(std::move(requestId))
    {}
} // namespace smart_home::usp_protocol::handlers