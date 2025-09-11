#include "../../../../include/handlers/version1/acknowledgement/AcknowledgementMessage.h"


namespace smart_home::usp_protocol::handlers::version1 {

    AcknowledgementMessage::AcknowledgementMessage(
        const ProtocolVersion& protocolVersion,
        const std::string& sessionId,
        const time_t& timestamp,
        const std::string& requestId,
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
} // namespace smart_home::usp_protocol::handlers::version1