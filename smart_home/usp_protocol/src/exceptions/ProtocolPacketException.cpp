#include "../../include/exceptions/ProtocolPacketException.h"


namespace smart_home::usp_protocol::exceptions {

    ProtocolPacketException::ProtocolPacketException (
        const ExceptionLevel level,
        const int errorCode,
        const char* message
    )
        : CompleteException(level, errorCode, message)
    {
        const std::string detailedMessage = std::string("ProtocolPacketException: \n")
            .append(CompleteException::message);

        const auto newMessage = new char[detailedMessage.size() + 1];
        std::strcpy(newMessage, detailedMessage.c_str());
        this->message = newMessage;
    }
} // namespace smart_home::usp_protocol::exceptions