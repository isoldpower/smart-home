#include "../../include/exceptions/LevelException.h"

#include <string>


namespace smart_home::utilities::exceptions {

    LevelException::LevelException(const ExceptionLevel level, const char* message)
        : message(message)
        , levelType(level)
    {}

    const char* LevelException::what() const noexcept {
        return message.c_str();
    }

    ExceptionLevel LevelException::getLevelType() const {
        return levelType;
    }

} // namespace smart_home::utilities::exceptions