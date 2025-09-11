#pragma once
#include "./messages/CommonMessageData.h"
#include "./messages/RequestMessage.h"


namespace smart_home::usp_protocol {

    struct UspServerRequest
        : public messages::CommonMessageData
        , public messages::RequestMessageData
    {
    public:
        UspServerRequest(
            const messages::ProtocolVersion& protocolVersion,
            const std::string& sessionId,
            const messages::MessageType& messageType,
            const time_t& timestamp,
            const std::string& requestId,
            const std::string& auth,
            const uint8_t& actionGroup,
            const uint8_t& action,
            const size_t& size,
            const std::string& data
        )
            : messages::CommonMessageData(
                protocolVersion,
                sessionId,
                messageType,
                timestamp,
                requestId
            )
            , messages::RequestMessageData(
                auth,
                actionGroup,
                action,
                size,
                data
            )
        {}
    };
} // namespace smart_home::usp_protocol