#pragma once

#include <array>
#include <string>

#include "../../model/Message.h"
#include "../BinaryMessage.h"


namespace smart_home::usp_protocol::version1 {

    enum class ResponseSegmentsIndex : size_t {
        VERSION_BYTE = static_cast<size_t>(CommonMessageIndexes::VERSION_BYTE),
        TYPE_BYTE = static_cast<size_t>(CommonMessageIndexes::TYPE_BYTE),
        SESSION_ID_START = static_cast<size_t>(CommonMessageIndexes::SESSION_ID_START),
        SESSION_ID_END = static_cast<size_t>(CommonMessageIndexes::SESSION_ID_END),
        REQUEST_ID_START = static_cast<size_t>(CommonMessageIndexes::REQUEST_ID_START),
        REQUEST_ID_END = static_cast<size_t>(CommonMessageIndexes::REQUEST_ID_END),
        TIMESTAMP_START = static_cast<size_t>(CommonMessageIndexes::TIMESTAMP_START),
        TIMESTAMP_END = static_cast<size_t>(CommonMessageIndexes::TIMESTAMP_END),
        PACKETS_COUNT_BYTE = static_cast<size_t>(CommonMessageIndexes::TIMESTAMP_END) + 1,
        PACKET_INDEX_BYTE = static_cast<size_t>(CommonMessageIndexes::TIMESTAMP_END) + 2,
        STATUS_BYTE = static_cast<size_t>(CommonMessageIndexes::TIMESTAMP_END) + 3,
        SIZE_BYTE = static_cast<size_t>(CommonMessageIndexes::TIMESTAMP_END) + 4,
        DATA_START = static_cast<size_t>(CommonMessageIndexes::TIMESTAMP_END) + 5,

        internal_SegmentsCount = 10
    };

    consteval size_t getResponseSegmentIndex(ResponseSegmentsIndex index) {
        return static_cast<size_t>(index);
    }

    inline constexpr int RESPONSE_CHUNKS_AMOUNT = static_cast<size_t>(
        ResponseSegmentsIndex::internal_SegmentsCount
    );

    using ResponseMessageRaw = BinaryMessageRaw<RESPONSE_CHUNKS_AMOUNT>;

    enum class ResponseStatus : uint8_t {
        STATUS_OK = 0x01,
        STATUS_ERROR = 0x02,
        STATUS_UNAUTHORIZED = 0x03,
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
            const uint64_t& timestamp,
            const uint16_t& requestId,
            const size_t& packetsCount,
            const size_t& packetIndex,
            const ResponseStatus& status,
            const size_t& size,
            const std::string& data
        );

        [[nodiscard]] bool isValid() const override;
    };
} // namespace smart_home::usp_protocol::version1