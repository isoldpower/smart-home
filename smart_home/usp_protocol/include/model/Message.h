#pragma once

#include "../CommonMessageData.h"

namespace smart_home::usp_protocol::model {

    class IValidatable {
    public:
        virtual ~IValidatable() = default;
        [[nodiscard]] virtual bool isValid() const = 0;
    };

    struct Message
        : public CommonMessageData
        , public IValidatable
    {
        explicit Message(
            const ProtocolVersion& protocolVersion,
            const uint16_t& sessionId,
            const MessageType& messageType,
            const uint64_t& timestamp,
            const uint16_t& requestId
        )
            : CommonMessageData(
                protocolVersion,
                sessionId,
                messageType,
                timestamp,
                requestId
            )
        {}
    };

} // namespace smart_home::usp_protocol::model