#include "../../include/exceptions/ProtocolStructuralException.h"


namespace smart_home::usp_protocol::exceptions {

    ProtocolStructuralException::ProtocolStructuralException (
        const utilities::exceptions::ExceptionLevel level,
        const int errorCode,
        const char* message
    )
        : CompleteException(level, errorCode, message)
    {
        this->messageRaw = std::string("ProtocolStructuralException: \n")
            .append(CompleteException::message);
    }
} // namespace smart_home::usp_protocol::exceptions