#pragma once

#include <array>
#include <string>

#include "../../model/Message.h"
#include "../BinaryMessage.h"


namespace smart_home::usp_protocol::version1 {

    enum class ProtocolSegmentsIndex : size_t {
        VERSION_BYTE = static_cast<size_t>(CommonMessageIndexes::VERSION_BYTE),
        TYPE_BYTE = static_cast<size_t>(CommonMessageIndexes::TYPE_BYTE),
        SESSION_ID_START = static_cast<size_t>(CommonMessageIndexes::SESSION_ID_START),
        SESSION_ID_END = static_cast<size_t>(CommonMessageIndexes::SESSION_ID_END),
        REQUEST_ID_START = static_cast<size_t>(CommonMessageIndexes::REQUEST_ID_START),
        REQUEST_ID_END = static_cast<size_t>(CommonMessageIndexes::REQUEST_ID_END),
        TIMESTAMP_START = static_cast<size_t>(CommonMessageIndexes::TIMESTAMP_START),
        TIMESTAMP_END = static_cast<size_t>(CommonMessageIndexes::TIMESTAMP_END),
        ACTION_BYTE = static_cast<size_t>(CommonMessageIndexes::TIMESTAMP_END) + 1,
        SIZE_BYTE = static_cast<size_t>(CommonMessageIndexes::TIMESTAMP_END) + 2,
        DATA_START = static_cast<size_t>(CommonMessageIndexes::TIMESTAMP_END) + 3,

        internal_SegmentsCount = 8
    };

    constexpr size_t getProtocolSegmentIndex(ProtocolSegmentsIndex index) {
        return static_cast<size_t>(index);
    }

    inline constexpr int PROTOCOL_CHUNKS_AMOUNT = static_cast<size_t>(
        ProtocolSegmentsIndex::internal_SegmentsCount
    );

    using ProtocolMessageRaw = BinaryMessageRaw<PROTOCOL_CHUNKS_AMOUNT>;

    enum class ProtocolAction : uint8_t {
        ACTION_HANDSHAKE = 0x01,
        ACTION_TERMINATE = 0x02,
        ACTION_HEARTBEAT = 0x03,
        ACTION_UNKNOWN = 0xFF,
    };

    struct ProtocolMessage final : public model::Message {
    public:
        ProtocolAction action;
        size_t size;
        std::string data;

        ProtocolMessage(
            const ProtocolVersion& protocolVersion,
            const uint16_t& sessionId,
            const uint64_t& timestamp,
            const uint16_t& requestId,
            const ProtocolAction& action,
            const size_t& size,
            std::string data
        );

        [[nodiscard]] bool isValid() const override;
    };

} // namespace smart_home::usp_protocol::version1