#include "../../include/exceptions/ReceivedMessageException.h"

namespace smart_home::usp_server::exceptions {

    ReceivedMessageException::ReceivedMessageException(
        const ExceptionLevel level,
        const ExecutionCodes errorCode,
        const char* message
    )
        : CompleteException(level, castExecutionCode(errorCode), message)
    {
        rawMessage = std::string("ReceivedMessageException (error in protocol structure): \n")
            .append(CompleteException::message);
    }

} // namespace smart_home::usp_server::exceptions
