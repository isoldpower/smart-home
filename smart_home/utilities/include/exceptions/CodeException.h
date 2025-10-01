#pragma once

#include <exception>
#include <string>


namespace smart_home::utilities::exceptions {

    class CodeException : public std::exception {
    protected:
        std::string message;
        const int errorCode;
    public:
        explicit CodeException(int errorCode, const char* message);
        [[nodiscard]] const char* what() const noexcept override;
    };
} // namespace smart_home::utilities::exceptions