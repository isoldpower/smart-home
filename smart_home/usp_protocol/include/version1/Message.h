#pragma once

#include "./CommonMessageData.h"


namespace smart_home::usp_protocol::version1 {

    class IValidatable {
    public:
        virtual ~IValidatable() = default;
        [[nodiscard]] virtual bool isValid() const = 0;
    };

    struct Message
        : public CommonMessagePacketData
        , public IValidatable
    {
        explicit Message(
            const ProtocolVersion& protocolVersion,
            const uint16_t& sessionId,
            const MessageType& messageType,
            const uint64_t& timestamp,
            const uint16_t& requestId,
            const size_t& packetIndex,
            const size_t& packetsCount
        )
            : CommonMessagePacketData(
                protocolVersion,
                sessionId,
                messageType,
                timestamp,
                requestId,
                packetIndex,
                packetsCount
            )
        {}
    };

} // namespace smart_home::usp_protocol::version1