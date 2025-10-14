#pragma once

#include <array>
#include <string>

#include "../BinaryMessage.h"
#include "../Message.h"


namespace smart_home::usp_protocol::version1 {

    enum class RequestSegmentsIndex : size_t {
        VERSION_BYTE = static_cast<size_t>(CommonMessageIndexes::VERSION_BYTE),
        TYPE_BYTE = static_cast<size_t>(CommonMessageIndexes::TYPE_BYTE),
        SESSION_ID_START = static_cast<size_t>(CommonMessageIndexes::SESSION_ID_START),
        SESSION_ID_END = static_cast<size_t>(CommonMessageIndexes::SESSION_ID_END),
        REQUEST_ID_START = static_cast<size_t>(CommonMessageIndexes::REQUEST_ID_START),
        REQUEST_ID_END = static_cast<size_t>(CommonMessageIndexes::REQUEST_ID_END),
        TIMESTAMP_START = static_cast<size_t>(CommonMessageIndexes::TIMESTAMP_START),
        TIMESTAMP_END = static_cast<size_t>(CommonMessageIndexes::TIMESTAMP_END),
        PACKET_INDEX_BYTE = static_cast<size_t>(CommonMessageIndexes::PACKET_INDEX_BYTE),
        PACKETS_COUNT_BYTE = static_cast<size_t>(CommonMessageIndexes::PACKETS_COUNT_BYTE),
        AUTH_START = static_cast<size_t>(CommonMessageIndexes::PACKETS_COUNT_BYTE) + 1,
        AUTH_END = static_cast<size_t>(CommonMessageIndexes::PACKETS_COUNT_BYTE) + 4,
        GROUP_BYTE = static_cast<size_t>(CommonMessageIndexes::PACKETS_COUNT_BYTE) + 5,
        ACTION_BYTE = static_cast<size_t>(CommonMessageIndexes::PACKETS_COUNT_BYTE) + 6,
        SIZE_BYTE = static_cast<size_t>(CommonMessageIndexes::PACKETS_COUNT_BYTE) + 7,
        DATA_START = static_cast<size_t>(CommonMessageIndexes::PACKETS_COUNT_BYTE) + 8,

        internal_SegmentsCount = 12
    };

    consteval size_t getRequestSegmentIndex(RequestSegmentsIndex index) {
        return static_cast<size_t>(index);
    }

    inline constexpr int REQUEST_CHUNKS_AMOUNT = static_cast<size_t>(
        RequestSegmentsIndex::internal_SegmentsCount
    );

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
        : public Message
        , public RequestMessageData
    {
    public:
        RequestMessage(
            const ProtocolVersion& protocolVersion,
            const uint16_t& sessionId,
            const uint64_t& timestamp,
            const uint16_t& requestId,
            const size_t& packetIndex,
            const size_t& packetsCount,
            std::string auth,
            const uint8_t& actionGroup,
            const uint8_t& action,
            const size_t& size,
            std::string data
        );

        [[nodiscard]] bool isValid() const override;
    };

} // namespace smart_home::usp_protocol::version1