#pragma once

#include <array>
#include <string>
#include <utility>

#include "../../model/Message.h"
#include "../BinaryMessage.h"


namespace smart_home::usp_protocol::handlers::version1 {

    enum class ProtocolSegmentsIndex : size_t {
        VERSION = 0,
        SESSION_ID = 1,
        MESSAGE_TYPE = 2,
        TIMESTAMP = 3,
        ACTION = 4,
        SIZE = 5,
        DATA = 6,

        internal_Count = 7
    };

    inline constexpr int PROTOCOL_CHUNKS_AMOUNT = static_cast<size_t>(ProtocolSegmentsIndex::internal_Count);

    using ProtocolMessageRaw = BinaryMessageRaw<PROTOCOL_CHUNKS_AMOUNT>;

    enum class ProtocolAction : uint8_t {
        ACTION_HANDSHAKE = 0x00,
        ACTION_TERMINATE = 0x01,
        ACTION_HEARTBEAT = 0x02,
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
            const time_t& timestamp,
            const uint16_t& requestId,
            const ProtocolAction& action,
            const size_t& size,
            std::string data
        );

        [[nodiscard]] bool isValid() const override;
    };

} // namespace smart_home::usp_protocol::handlers::version1