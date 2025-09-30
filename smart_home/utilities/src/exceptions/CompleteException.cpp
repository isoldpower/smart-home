#include "../../include/exceptions/CompleteException.h"

#include <string>


namespace smart_home::utilities::exceptions {

    CompleteException::CompleteException(
        const ExceptionLevel level,
        const int errorCode,
        const char* message
    )
        : CodeException(errorCode, message)
        , LevelException(level, message)
    {
        const std::string levelVerbose = getExceptionLevelVerbose(level);
        const std::string messagePrefix = "[" + levelVerbose + " #" + std::to_string(errorCode) + "]";
        const std::string finalMessage = messagePrefix + " " + message;

        const auto newString = new char[finalMessage.size() + 1];
        strcpy(newString, finalMessage.c_str());
        this->message = newString;
    }

    const char* CompleteException::what() const noexcept {
        return message;
    }

} // namespace smart_home::utilities::exceptions