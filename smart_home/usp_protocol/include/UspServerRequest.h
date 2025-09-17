#pragma once

#include <ostream>

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
            const uint16_t& sessionId,
            const handlers::MessageType& messageType,
            const time_t& timestamp,
            const uint16_t& requestId,
            const std::string& auth,
            const uint8_t& actionGroup,
            const uint8_t& action,
            const size_t& size,
            const std::string& data
        );

        friend std::ostream& operator<<(std::ostream& os, const UspServerRequest& req);
    };

    inline std::ostream& operator<<(std::ostream& os, const UspServerRequest& req) {
        os << "version: " << static_cast<int>(req.protocolVersion) << "\n"
           << "sessionId: " << req.sessionId << "\n"
           << "type: " << static_cast<int>(req.type) << "\n"
           << "timestamp: " << req.timestamp << "\n"
           << "requestId: " << req.requestId << "\n"
           << "auth: " << req.auth << "\n"
           << "actionGroup: " << static_cast<int>(req.actionGroup) << "\n"
           << "action: " << static_cast<int>(req.action) << "\n"
           << "size: " << req.size << "\n"
           << "data: " << req.data << "\n";
        return os;
    }
} // namespace smart_home::usp_protocol