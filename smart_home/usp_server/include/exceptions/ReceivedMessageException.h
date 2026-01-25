#pragma once

#include <smart_home/utilities/include/exceptions/CompleteException.h>
#include "./ExecutionCodes.h"


namespace smart_home::usp_server::exceptions {
    using namespace utilities::exceptions;

    class ReceivedMessageException
        : public CompleteException
    {
    private:
        std::string rawMessage;
    public:
        explicit ReceivedMessageException(
            ExceptionLevel level,
            ExecutionCodes errorCode,
            const char* message
        );
    };

} // namespace smart_home::usp_server::exceptions