#pragma once

#include <array>
#include <string>

#include "../../model/Message.h"
#include "../BinaryMessage.h"


namespace smart_home::usp_protocol::handlers::version1 {

    enum class ResponseSegmentsIndex : size_t {
        VERSION = 0,
        SESSION_ID = 1,
        MESSAGE_TYPE = 2,
        TIMESTAMP = 3,
        PACKETS_COUNT = 4,
        PACKET_INDEX = 5,
        REQUEST_ID = 6,
        STATUS = 7,
        SIZE = 8,
        DATA = 9,

        internal_Count = 10
    };

    inline constexpr int RESPONSE_CHUNKS_AMOUNT = static_cast<size_t>(ResponseSegmentsIndex::internal_Count);

    using ResponseMessageRaw = BinaryMessageRaw<RESPONSE_CHUNKS_AMOUNT>;

    enum class ResponseStatus : uint8_t {
        STATUS_OK = 0x00,
        STATUS_ERROR = 0x01,
        STATUS_UNAUTHORIZED = 0x02,
        STATUS_UNKNOWN = 0xFF
    };

    struct ResponseMessageData {
    public:
        ResponseStatus status;
        size_t size;
        std::string data;

        explicit ResponseMessageData(
            const ResponseStatus& status,
            const size_t& size,
            std::string data
        );
    };

    struct ResponseMessage
        : public model::Message
        , public ResponseMessageData
        , public PacketMessage
    {
    public:
        explicit ResponseMessage(
            const ProtocolVersion& protocolVersion,
            const uint16_t& sessionId,
            const time_t& timestamp,
            const uint16_t& requestId,
            const size_t& packetsCount,
            const size_t& packetIndex,
            const ResponseStatus& status,
            const size_t& size,
            const std::string& data
        );

        [[nodiscard]] bool isValid() const override;
    };
} // namespace smart_home::usp_protocol::handlers::version1