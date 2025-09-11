#pragma once

#include <array>
#include <string>
#include <utility>

#include "./BinaryMessage.h"
#include "./model/Message.h"


namespace smart_home::usp_protocol::messages {

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
            const std::string& sessionId,
            const time_t& timestamp,
            const std::string& requestId,
            const ProtocolAction& action,
            const size_t& size,
            std::string data
        )
            : model::Message(
                protocolVersion,
                sessionId,
                MessageType::MESSAGE_PROTOCOL,
                timestamp,
                requestId
            )
            , action(action)
            , size(size)
            , data(std::move(data))
        {}

        [[nodiscard]] bool isValid() const override;
    };

} // namespace smart_home::usp_protocol::messages