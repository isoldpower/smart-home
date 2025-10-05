#pragma once

#include <smart_home/usp_protocol/include/version1/CommonMessageData.h>
#include <smart_home/usp_protocol/include/version1/response/ResponseMessage.h>
#include <ostream>


namespace smart_home::usp_server {

    using namespace smart_home::usp_protocol;

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

    inline std::ostream& operator<<(std::ostream& os, const UspServerResponse& res) {
        os << "version: " << static_cast<int>(res.protocolVersion) << "\n"
           << "sessionId: " << res.sessionId << "\n"
           << "type: " << static_cast<int>(res.type) << "\n"
           << "timestamp: " << res.timestamp << "\n"
           << "requestId: " << res.requestId << "\n"
           << "status: " << static_cast<int>(res.status) << "\n"
           << "size: " << res.size << "\n"
           << "data: " << res.data << "\n";

        return os;
    }

} // namespace smart_home::usp_server