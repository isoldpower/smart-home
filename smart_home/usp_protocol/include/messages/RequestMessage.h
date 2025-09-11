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
        )
            : auth(std::move(auth))
            , actionGroup(actionGroup)
            , action(action)
            , size(size)
            , data(std::move(data))
        {}
    };

    struct RequestMessage final
        : public model::Message
        , public RequestMessageData
        , public PacketMessage
    {
    public:
        size_t packetsCount;
        size_t packetIndex;
        std::string auth;
        uint8_t actionGroup;
        uint8_t action;
        size_t size;
        std::string data;

        RequestMessage(
            const ProtocolVersion& protocolVersion,
            const std::string& sessionId,
            const time_t& timestamp,
            const std::string& requestId,
            const size_t& packetsCount,
            const size_t& packetIndex,
            std::string auth,
            const uint8_t& actionGroup,
            const uint8_t& action,
            const size_t& size,
            std::string data
        )
            : model::Message(
                protocolVersion,
                sessionId,
                MessageType::MESSAGE_REQUEST,
                timestamp,
                requestId
            )
            , RequestMessageData(
                std::move(auth),
                actionGroup,
                action,
                size,
                std::move(data)
            )
            , PacketMessage(packetsCount, packetIndex)
        {}

        [[nodiscard]] bool isValid() const override;
    };

} // namespace smart_home::usp_protocol::messages