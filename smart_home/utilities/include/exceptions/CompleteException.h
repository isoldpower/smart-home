#pragma once

#include "./CodeException.h"
#include "./LevelException.h"


namespace smart_home::utilities::exceptions {

    class CompleteException
        : public CodeException
        , public LevelException
    {
    private:
        std::string messageRaw;
    protected:
        const char* message;
    public:
        explicit CompleteException(ExceptionLevel level, int errorCode, const char* message);
        [[nodiscard]] const char* what() const noexcept override;
    };
} // namespace smart_home::utilities::exceptions