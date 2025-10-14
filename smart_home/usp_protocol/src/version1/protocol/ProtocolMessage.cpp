#include "../../../include/version1/protocol/ProtocolMessage.h"


namespace smart_home::usp_protocol::version1 {

    ProtocolMessage::ProtocolMessage(
        const ProtocolVersion& protocolVersion,
        const uint16_t& sessionId,
        const uint64_t& timestamp,
        const uint16_t& requestId,
        const size_t& packetIndex,
        const size_t& packetsCount,
        const ProtocolAction& action,
        const size_t& size,
        std::string data
    )
        : Message(
            protocolVersion,
            sessionId,
            MessageType::MESSAGE_PROTOCOL,
            timestamp,
            requestId,
            packetIndex,
            packetsCount
        )
        , action(action)
        , size(size)
        , data(std::move(data))
    {}

    bool ProtocolMessage::isValid() const {
        return true;
    }
} // namespace smart_home::usp_protocol::version1