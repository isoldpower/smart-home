#include "../../../include/version1/acknowledgement/AcknowledgementMessage.h"


namespace smart_home::usp_protocol::version1 {

    AcknowledgementMessage::AcknowledgementMessage(
        const ProtocolVersion& protocolVersion,
        const uint16_t& sessionId,
        const uint64_t& timestamp,
        const uint16_t& requestId,
        const AcknowledgementStatus& status,
        const size_t& size,
        std::string data
    )
        : Message(
            protocolVersion,
            sessionId,
            MessageType::MESSAGE_ACKNOWLEDGEMENT,
            timestamp,
            requestId
        )
        , status(status)
        , size(size)
        , data(std::move(data))
    {}

    bool AcknowledgementMessage::isValid() const {
        return true;
    }
} // namespace smart_home::usp_protocol::version1