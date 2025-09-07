#pragma once

#include <array>
#include <string>
#include <utility>

#include "./BinaryMessage.h"
#include "./model/Message.h"


namespace smart_home::usp_protocol::messages {

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

    struct RequestMessage final : public model::Message {
    public:
        size_t packetsCount;
        size_t packetIndex;
        std::string requestId;
        std::string auth;
        uint8_t actionGroup;
        uint8_t action;
        size_t size;
        std::string data;

        RequestMessage(
            const ProtocolVersion& protocolVersion,
            std::string sessionId,
            const time_t& timestamp,
            const size_t& packetsCount,
            const size_t& packetIndex,
            std::string requestId,
            std::string auth,
            const uint8_t& actionGroup,
            const uint8_t& action,
            const size_t& size,
            std::string data
        )
            : model::Message(protocolVersion, std::move(sessionId), MessageType::MESSAGE_REQUEST, timestamp)
            , packetsCount(packetsCount)
            , packetIndex(packetIndex)
            , requestId(std::move(requestId))
            , auth(std::move(auth))
            , actionGroup(actionGroup)
            , action(action)
            , size(size)
            , data(std::move(data))
        {}

        [[nodiscard]] bool isValid() const override;
    };

} // namespace smart_home::usp_protocol::messages