#pragma once

#include <array>
#include <string>

#include "../../model/Message.h"
#include "../BinaryMessage.h"


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
        PACKETS_COUNT_BYTE = static_cast<size_t>(CommonMessageIndexes::TIMESTAMP_END) + 1,
        PACKET_INDEX_BYTE = static_cast<size_t>(CommonMessageIndexes::TIMESTAMP_END) + 2,
        AUTH_START = static_cast<size_t>(CommonMessageIndexes::TIMESTAMP_END) + 3,
        AUTH_END = static_cast<size_t>(CommonMessageIndexes::TIMESTAMP_END) + 6,
        GROUP_BYTE = static_cast<size_t>(CommonMessageIndexes::TIMESTAMP_END) + 7,
        ACTION_BYTE = static_cast<size_t>(CommonMessageIndexes::TIMESTAMP_END) + 8,
        SIZE_BYTE = static_cast<size_t>(CommonMessageIndexes::TIMESTAMP_END) + 9,
        DATA_START = static_cast<size_t>(CommonMessageIndexes::TIMESTAMP_END) + 10,

        internal_SegmentsCount = 12
    };

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
        : public model::Message
        , public RequestMessageData
        , public PacketMessage
    {
    public:
        RequestMessage(
            const ProtocolVersion& protocolVersion,
            const uint16_t& sessionId,
            const uint64_t& timestamp,
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

} // namespace smart_home::usp_protocol::version1