#pragma once

#include <array>
#include <string>

#include "../../model/Message.h"
#include "../BinaryMessage.h"


namespace smart_home::usp_protocol::handlers::version1 {

    enum class RequestSegmentsIndex : size_t {
        VERSION = 0,
        SESSION_ID = 1,
        MESSAGE_TYPE = 2,
        TIMESTAMP = 3,
        PACKETS_COUNT = 4,
        PACKET_INDEX = 5,
        REQUEST_ID = 6,
        AUTH = 7,
        GROUP = 8,
        ACTION = 9,
        SIZE = 10,
        DATA = 11,

        internal_Count = 12
    };

    inline constexpr int REQUEST_CHUNKS_AMOUNT = static_cast<size_t>(RequestSegmentsIndex::internal_Count);

    using RequestMessageRaw = BinaryMessageRaw<REQUEST_CHUNKS_AMOUNT>;

    struct RequestMessageData {
    public:
        std::string auth;
        uint8_t actionGroup;
        uint8_t action;
        size_t size;
        std::string data;

        RequestMessageData(
            std::string auth,
            const uint8_t& actionGroup,
            const uint8_t& action,
            const size_t& size,
            std::string data
        );
    };

    struct RequestMessage final
        : public model::Message
        , public RequestMessageData
        , public PacketMessage
    {
    public:
        RequestMessage(
            const ProtocolVersion& protocolVersion,
            const uint16_t& sessionId,
            const time_t& timestamp,
            const uint16_t& requestId,
            const size_t& packetsCount,
            const size_t& packetIndex,
            std::string auth,
            const uint8_t& actionGroup,
            const uint8_t& action,
            const size_t& size,
            std::string data
        );

        [[nodiscard]] bool isValid() const override;
    };

} // namespace smart_home::usp_protocol::handlers::version1