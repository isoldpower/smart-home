#include "../../include/exceptions/ProtocolSerializationException.h"


namespace smart_home::usp_protocol::exceptions {

    ProtocolSerializationException::ProtocolSerializationException (
        const utilities::exceptions::ExceptionLevel level,
        const int errorCode,
        const char* message
    )
        : CompleteException(level, errorCode, message)
    {
        this->messageRaw = std::string("ProtocolSerializationException: \n")
            .append(CompleteException::message);
    }
} // namespace smart_home::usp_protocol::exceptions