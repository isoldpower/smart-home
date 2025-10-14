#include "../../include/version1/UspServerRequest.h"

namespace smart_home::usp_server::version1 {

    UspServerRequest::UspServerRequest(
        const usp_protocol::ProtocolVersion& protocolVersion,
        const uint16_t& sessionId,
        const usp_protocol::version1::MessageType& messageType,
        const time_t& timestamp,
        const uint16_t& requestId,
        const std::string& auth,
        const uint8_t& actionGroup,
        const uint8_t& action,
        const size_t& size,
        const std::string& data
    )
        : usp_protocol::version1::CommonMessageData(
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
} // namespace smart_home::usp_server::version1