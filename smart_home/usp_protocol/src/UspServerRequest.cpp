#include "../include/UspServerRequest.h"


namespace smart_home::usp_protocol {

    UspServerRequest::UspServerRequest(
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
} // namespace smart_home::usp_protocol