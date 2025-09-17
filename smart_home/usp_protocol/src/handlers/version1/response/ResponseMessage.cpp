#include "../../../../include/handlers/version1/response/ResponseMessage.h"


namespace smart_home::usp_protocol::handlers::version1 {

    ResponseMessageData::ResponseMessageData(
        const ResponseStatus& status,
        const size_t& size,
        std::string data
    )
        : status(status)
        , size(size)
        , data(std::move(data))
    {}

    ResponseMessage::ResponseMessage(
        const ProtocolVersion& protocolVersion,
        const uint16_t& sessionId,
        const time_t& timestamp,
        const uint16_t& requestId,
        const size_t& packetsCount,
        const size_t& packetIndex,
        const ResponseStatus& status,
        const size_t& size,
        const std::string& data
    )
        : model::Message(
            protocolVersion,
            sessionId,
            MessageType::MESSAGE_REQUEST,
            timestamp,
            requestId
        )
        , ResponseMessageData(
            status,
            size,
            data
        )
        , PacketMessage(packetsCount, packetIndex)
    {}

    bool ResponseMessage::isValid() const {
        return true;
    }
} // namespace smart_home::usp_protocol::handlers::version1