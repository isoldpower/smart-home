#pragma once

#include <smart_home/utilities/include/exceptions/CompleteException.h>
#include <string>


namespace smart_home::usp_protocol::exceptions {

    class ProtocolStructuralException
        : public utilities::exceptions::CompleteException
    {
    private:
        std::string messageRaw;
    public:
        explicit ProtocolStructuralException(
            utilities::exceptions::ExceptionLevel level,
            int errorCode,
            const char* message
        );
    };
} // namespace smart_home::usp_protocol::exceptions