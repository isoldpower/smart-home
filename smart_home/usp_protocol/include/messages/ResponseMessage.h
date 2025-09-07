#pragma once

#include <array>
#include <string>
#include <utility>

#include "./BinaryMessage.h"
#include "./model/Message.h"


namespace smart_home::usp_protocol::messages {

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

    struct ResponseMessage : public model::Message {
    public:
        size_t packetsCount;
        size_t packetIndex;
        std::string requestId;
        ResponseStatus status;
        size_t size;
        std::string data;

        explicit ResponseMessage(
            const ProtocolVersion& protocolVersion,
            std::string sessionId,
            const time_t& timestamp,
            const size_t& packetsCount,
            const size_t& packetIndex,
            std::string requestId,
            const ResponseStatus& status,
            const size_t& size,
            std::string data
        )
            : model::Message(protocolVersion, std::move(sessionId), MessageType::MESSAGE_REQUEST, timestamp)
            , packetsCount(packetsCount)
            , packetIndex(packetIndex)
            , requestId(std::move(requestId))
            , status(status)
            , size(size)
            , data(std::move(data))
        {}

        [[nodiscard]] bool isValid() const override;
    };
} // namespace smart_home::usp_protocol