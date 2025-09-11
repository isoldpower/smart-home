#pragma once

#include "handlers/version1/response/ResponseMessage.h"

namespace smart_home::usp_protocol {

    // TODO: Fix that server is hardly bounded to specific response version
    struct UspServerResponse
        : public handlers::CommonMessageData
        , public handlers::version1::ResponseMessageData
    {
    public:
        UspServerResponse(
            const handlers::ProtocolVersion& protocolVersion,
            const std::string& sessionId,
            const time_t& timestamp,
            const std::string& requestId,
            const handlers::version1::ResponseStatus& status,
            const size_t& size,
            const std::string& data
        );
    };

} // namespace smart_home::usp_protocol