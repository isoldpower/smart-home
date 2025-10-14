#include "../../include/version1/UspServerResponse.h"

namespace smart_home::usp_server::version1 {

    UspServerResponse::UspServerResponse(
        const usp_protocol::ProtocolVersion& protocolVersion,
        const uint16_t& sessionId,
        const time_t& timestamp,
        const uint16_t& requestId,
        const usp_protocol::version1::ResponseStatus& status,
        const size_t& size,
        const std::string& data
    )
        : usp_protocol::version1::CommonMessageData(
            protocolVersion,
            sessionId,
            usp_protocol::version1::MessageType::MESSAGE_RESPONSE,
            timestamp,
            requestId
        )
        , ResponseMessageData(
            status,
            size,
            data
        )
    {}
} // namespace smart_home::usp_server::version1