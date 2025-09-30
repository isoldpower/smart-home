#pragma once

#include <smart_home/utilities/include/exceptions/CompleteException.h>


namespace smart_home::usp_protocol::exceptions {
    using namespace utilities::exceptions;

    class ProtocolPacketException
        : public CompleteException
    {
    public:
        explicit ProtocolPacketException(
            ExceptionLevel level,
            int errorCode,
            const char* message
        );
    };
} // namespace smart_home::usp_protocol::exceptions