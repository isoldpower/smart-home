#pragma once

#include <smart_home/usp_protocol/include/version1/response/ResponseMessage.h>


namespace smart_home::usp_server {

    using namespace smart_home::usp_protocol;

    // TODO: Fix that server is hardly bounded to specific response version
    struct UspServerResponse
        : public CommonMessageData
        , public version1::ResponseMessageData
    {
    public:
        UspServerResponse(
            const ProtocolVersion& protocolVersion,
            const uint16_t& sessionId,
            const time_t& timestamp,
            const uint16_t& requestId,
            const version1::ResponseStatus& status,
            const size_t& size,
            const std::string& data
        );
    };

} // namespace smart_home::usp_server