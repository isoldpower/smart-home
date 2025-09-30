#include "../../include/exceptions/ProtocolStructuralException.h"


namespace smart_home::usp_protocol::exceptions {

    ProtocolStructuralException::ProtocolStructuralException (
        const ExceptionLevel level,
        const int errorCode,
        const char* message
    )
        : CompleteException(level, errorCode, message)
    {
        const std::string detailedMessage = std::string("ProtocolStructuralException: \n")
            .append(CompleteException::message);

        const auto newMessage = new char[detailedMessage.size() + 1];
        std::strcpy(newMessage, detailedMessage.c_str());
        this->message = newMessage;
    }
} // namespace smart_home::usp_protocol::exceptions