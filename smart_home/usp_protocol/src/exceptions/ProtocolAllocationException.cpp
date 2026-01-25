#include "../../include/exceptions/ProtocolAllocationException.h"


namespace smart_home::usp_protocol::exceptions {

    ProtocolAllocationException::ProtocolAllocationException (
        const utilities::exceptions::ExceptionLevel level,
        const int errorCode,
        const char* message
    )
        : CompleteException(level, errorCode, message)
    {
        rawMessage = std::string("ProtocolAllocationException: \n")
            .append(CompleteException::message);
    }
} // namespace smart_home::usp_protocol::exceptions