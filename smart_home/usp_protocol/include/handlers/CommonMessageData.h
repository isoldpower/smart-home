#pragma once

#include <algorithm>
#include <string>

#include "./CommonOptions.h"


namespace smart_home::usp_protocol::handlers {

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
        std::string sessionId;
        MessageType type;
        std::time_t timestamp;
        std::string requestId;

        CommonMessageData(
            const ProtocolVersion& protocolVersion,
            std::string sessionId,
            const MessageType& messageType,
            const time_t& timestamp,
            std::string requestId
        );
    };
} // namespace smart_home::usp_protocol::messages