#include <utility>

#include "../../../../include/handlers/version1/request/RequestMessage.h"

namespace smart_home::usp_protocol::handlers::version1 {

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

    bool RequestMessage::isValid() const {
        return true;
    }
} // namespace smart_home::usp_protocol::handlers::version1