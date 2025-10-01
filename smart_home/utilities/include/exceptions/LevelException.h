#pragma once

#include <exception>
#include <string>


namespace smart_home::utilities::exceptions {
    enum ExceptionLevel {
        INFO,
        WARNING,
        ERROR,
        FATAL
    };

    inline const char* getExceptionLevelVerbose(const ExceptionLevel level) {
        switch (level) {
            case INFO:
                return "INFO";
            case WARNING:
                return "WARNING";
            case ERROR: default:
                return "ERROR";
            case FATAL:
                return "FATAL";
        }
    }

    class LevelException : public std::exception {
    protected:
        const std::string message;
        ExceptionLevel levelType;
    public:
        explicit LevelException(ExceptionLevel level, const char* message);

        [[nodiscard]] ExceptionLevel getLevelType() const;
        [[nodiscard]] const char* what() const noexcept override;
    };
} // smart_home::utilities::exceptions