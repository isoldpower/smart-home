#pragma once

#include <algorithm>
#include <string>
#include <utility>


namespace smart_home::usp_protocol::messages {

    enum class ProtocolVersion : uint8_t {
        VERSION_1 = 1,
    };

    enum class MessageType : uint8_t {
        MESSAGE_REQUEST = 0x00,
        MESSAGE_RESPONSE = 0x01,
        MESSAGE_ACKNOWLEDGEMENT = 0x02,
        MESSAGE_PROTOCOL = 0x03,
        MESSAGE_UNKNOWN = 0xFF,
    };

    struct CommonMessageData {
    public:
        ProtocolVersion protocolVersion;
        std::string sessionId;
        MessageType type;
        std::time_t timestamp;

        CommonMessageData(
            const ProtocolVersion& protocolVersion,
            std::string sessionId,
            const MessageType& messageType,
            const time_t& timestamp
        )
            : protocolVersion(protocolVersion)
            , sessionId(std::move(sessionId))
            , type(messageType)
            , timestamp(timestamp)
        {}
    };
} // namespace smart_home::usp_protocol::messages