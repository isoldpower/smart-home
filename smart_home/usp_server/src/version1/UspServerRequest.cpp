#include "../../include/version1/UspServerRequest.h"

namespace smart_home::usp_server {

    UspServerRequest::UspServerRequest(
        const ProtocolVersion& protocolVersion,
        const uint16_t& sessionId,
        const MessageType& messageType,
        const time_t& timestamp,
        const uint16_t& requestId,
        const std::string& auth,
        const uint8_t& actionGroup,
        const uint8_t& action,
        const size_t& size,
        const std::string& data
    )
        : CommonMessageData(
            protocolVersion,
            sessionId,
            messageType,
            timestamp,
            requestId
        )
        , RequestMessageData(
            auth,
            actionGroup,
            action,
            size,
            data
        )
    {}
} // namespace smart_home::usp_server