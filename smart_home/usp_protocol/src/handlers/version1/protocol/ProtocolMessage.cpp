#include "../../../../include/handlers/version1/protocol/ProtocolMessage.h"


namespace smart_home::usp_protocol::handlers::version1 {

    ProtocolMessage::ProtocolMessage(
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

    bool ProtocolMessage::isValid() const {
        return true;
    }
} // namespace smart_home::usp_protocol::handlers::version1