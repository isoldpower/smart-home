#pragma once

#include <functional>

#include "./CompleteException.h"


namespace smart_home::utilities::exceptions {

    class ExceptionsLoggerShell {
    private:
        std::function<bool()> isRunningSignal;
        std::vector<
            std::pair<int, std::function<void(CompleteException)>>
        > middlewares;

        void processUnhandledException(const CompleteException& exception) const;
    public:
        void addMiddleware(
            const std::function<void(CompleteException)>& middleware,
            int priority = 0
        );

        void safeRunProcedure(
            const char* procedureName,
            const std::function<void()>& procedure
        ) const;
    };

} // namespace smart_home::usp_protocol::utilities::exceptions