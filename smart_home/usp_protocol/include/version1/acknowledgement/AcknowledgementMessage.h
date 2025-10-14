#pragma once

#include <array>
#include <string>

#include "../BinaryMessage.h"
#include "../Message.h"


namespace smart_home::usp_protocol::version1 {

    enum class AcknowledgementSegmentsIndex : size_t {
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
        STATUS_BYTE = static_cast<size_t>(CommonMessageIndexes::PACKETS_COUNT_BYTE) + 1,
        SIZE_BYTE = static_cast<size_t>(CommonMessageIndexes::PACKETS_COUNT_BYTE) + 2,
        DATA_START = static_cast<size_t>(CommonMessageIndexes::PACKETS_COUNT_BYTE) + 3,

        internal_SegmentsCount = 10,
    };

    consteval size_t getAcknowledgementSegmentIndex(AcknowledgementSegmentsIndex index) {
        return static_cast<size_t>(index);
    }

    inline constexpr int ACKNOWLEDGEMENT_CHUNKS_AMOUNT = static_cast<size_t>(
        AcknowledgementSegmentsIndex::internal_SegmentsCount
    );

    using AcknowledgementMessageRaw = BinaryMessageRaw<ACKNOWLEDGEMENT_CHUNKS_AMOUNT>;

    enum class AcknowledgementStatus : uint8_t {
        ACKNOWLEDGEMENT_SUCCESS = 0x01,
        ACKNOWLEDGEMENT_ERROR_TIMEOUT = 0x02,
        ACKNOWLEDGEMENT_ERROR_NOACK = 0x03,
        ACKNOWLEDGEMENT_UNKNOWN = 0xFF,
    };

    struct AcknowledgementMessage final : public Message {
    public:
        AcknowledgementStatus status;
        size_t size;
        std::string data;

        explicit AcknowledgementMessage(
            const ProtocolVersion& protocolVersion,
            const uint16_t& sessionId,
            const uint64_t& timestamp,
            const uint16_t& requestId,
            const size_t& packetIndex,
            const size_t& packetsCount,
            const AcknowledgementStatus& status,
            const size_t& size = 0,
            std::string data = ""
        );

        [[nodiscard]] bool isValid() const override;
    };

} // namespace smart_home::usp_protocol::version1