#pragma once

#include <smart_home/utilities/include/exceptions/CompleteException.h>


namespace smart_home::usp_protocol::exceptions {

    class ProtocolAllocationException
        : public utilities::exceptions::CompleteException
    {
    private:
        std::string rawMessage;
    public:
        explicit ProtocolAllocationException(
            utilities::exceptions::ExceptionLevel level,
            int errorCode,
            const char* message
        );
    };
} // namespace smart_home::usp_protocol::exceptions