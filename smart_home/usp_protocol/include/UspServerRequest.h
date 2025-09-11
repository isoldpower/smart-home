#pragma once

#include "./handlers/CommonMessageData.h"
#include "./handlers/version1/request/RequestMessage.h"


namespace smart_home::usp_protocol {

    // TODO: Fix that server is hardly bounded to specific request version
    struct UspServerRequest
        : public handlers::CommonMessageData
        , public handlers::version1::RequestMessageData
    {
    public:
        UspServerRequest(
            const handlers::ProtocolVersion& protocolVersion,
            const std::string& sessionId,
            const handlers::MessageType& messageType,
            const time_t& timestamp,
            const std::string& requestId,
            const std::string& auth,
            const uint8_t& actionGroup,
            const uint8_t& action,
            const size_t& size,
            const std::string& data
        );
    };
} // namespace smart_home::usp_protocol