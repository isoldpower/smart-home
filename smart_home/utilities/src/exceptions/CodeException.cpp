#include "../../include/exceptions/CodeException.h"

#include <string>

namespace smart_home::utilities::exceptions {

    CodeException::CodeException(const int errorCode, const char* message)
        : errorCode(errorCode)
    {
        const std::string prefix = "[ERROR] Error #" + std::to_string(errorCode);
        this->message = prefix + ": " + message;
    }

    const char* CodeException::what() const noexcept {
        return message.c_str();
    }

} // namespace smart_home::utilities::exceptions