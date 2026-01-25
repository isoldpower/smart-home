#pragma once

#include <smart_home/utilities/include/exceptions/CompleteException.h>


namespace smart_home::usp_protocol::exceptions {

    class ProtocolSerializationException
        : public utilities::exceptions::CompleteException
    {
    private:
        std::string messageRaw;
    public:
        explicit ProtocolSerializationException(
            utilities::exceptions::ExceptionLevel level,
            int errorCode,
            const char* message
        );
    };
} // namespace smart_home::usp_protocol::exceptions