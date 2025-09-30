#include "../../include/exceptions/ProtocolSerializationException.h"


namespace smart_home::usp_protocol::exceptions {

    ProtocolSerializationException::ProtocolSerializationException (
        const ExceptionLevel level,
        const int errorCode,
        const char* message
    )
        : CompleteException(level, errorCode, message)
    {
        const std::string detailedMessage = std::string("ProtocolSerializationException: \n")
            .append(CompleteException::message);

        const auto newMessage = new char[detailedMessage.size() + 1];
        std::strcpy(newMessage, detailedMessage.c_str());
        this->message = newMessage;
    }
} // namespace smart_home::usp_protocol::exceptions