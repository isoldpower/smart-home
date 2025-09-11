#pragma once

#include "smart_home/usp_protocol/include/messages/CommonMessageData.h"


namespace smart_home::usp_protocol::messages::model {

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
            std::string sessionId,
            const MessageType& messageType,
            const time_t& timestamp,
            std::string requestId
        )
            : CommonMessageData(
                protocolVersion,
                std::move(sessionId),
                messageType,
                timestamp,
                std::move(requestId)
            )
        {}
    };

} // namespace smart_home::usp_protocol::messages::model