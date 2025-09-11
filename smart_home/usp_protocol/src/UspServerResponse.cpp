#include "../include/UspServerResponse.h"


namespace smart_home::usp_protocol {

    UspServerResponse::UspServerResponse(
        const handlers::ProtocolVersion& protocolVersion,
        const std::string& sessionId,
        const time_t& timestamp,
        const std::string& requestId,
        const handlers::version1::ResponseStatus& status,
        const size_t& size,
        const std::string& data
    )
        : CommonMessageData(
            protocolVersion,
            sessionId,
            handlers::MessageType::MESSAGE_RESPONSE,
            timestamp,
            requestId
        )
        , ResponseMessageData(
            status,
            size,
            data
        )
    {}
} // namespace smart_home::usp_protocol