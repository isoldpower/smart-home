#include <utility>

#include "../../../include/version1/request/RequestMessage.h"


namespace smart_home::usp_protocol::version1 {

    RequestMessageData::RequestMessageData(
        std::string  auth,
        const uint8_t& actionGroup,
        const uint8_t& action,
        const size_t& size,
        std::string  data
    )
        : auth(std::move(auth))
        , actionGroup(actionGroup)
        , action(action)
        , size(size)
        , data(std::move(data))
    {}

    RequestMessage::RequestMessage(
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

    bool RequestMessage::isValid() const {
        return true;
    }
} // namespace smart_home::usp_protocol::version1