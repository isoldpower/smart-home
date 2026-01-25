#include "../../include/exceptions/ProtocolPacketException.h"


namespace smart_home::usp_protocol::exceptions {

    ProtocolPacketException::ProtocolPacketException (
        const utilities::exceptions::ExceptionLevel level,
        const int errorCode,
        const char* message
    )
        : CompleteException(level, errorCode, message)
    {
        rawMessage = std::string("ProtocolPacketException: \n")
            .append(CompleteException::message);
    }
} // namespace smart_home::usp_protocol::exceptions