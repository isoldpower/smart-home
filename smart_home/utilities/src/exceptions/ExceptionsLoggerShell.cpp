#include "../../include/exceptions/ExceptionsLoggerShell.h"

#include <iostream>
#include <ranges>
#include <algorithm>

#include "../../include/exceptions/CompleteException.h"


namespace smart_home::utilities::exceptions {

    void ExceptionsLoggerShell::safeRunProcedure(
        const char *procedureName,
        const std::function<void()> &procedure
    ) const {
        try {
            procedure();
        } catch (const CompleteException& exception) {
            std::cerr << "Unhandled exception in procedure '"
                      << procedureName
                      << "'. Passing to middleware handlers..."
                      << std::endl;
            processUnhandledException(exception);
        } catch (const std::exception& exception) {
            std::cout << "Standard exception received: " << exception.what() << std::endl;
        } catch (...) {
            std::cout << "Unknown exception received" << std::endl;
        }
    }

    void ExceptionsLoggerShell::processUnhandledException(
        const CompleteException& exception
    ) const {
        for (const auto& middlewareCallback : middlewares | std::views::values) {
            middlewareCallback(exception);
        }

        std::cout << exception.what() << std::endl;
        switch (exception.getLevelType()) {
            case ExceptionLevel::INFO:
                std::cout << "Info received" << std::endl;
                break;
            case ExceptionLevel::WARNING:
                std::cout << "Warning received" << std::endl;
                break;
            case ExceptionLevel::ERROR:
                std::cout << "Error received" << std::endl;
                break;
            case ExceptionLevel::FATAL:
                std::cout << "Fatal Error received" << std::endl;
                throw;
        }
    }

    void ExceptionsLoggerShell::addMiddleware(
        const std::function<void(CompleteException)>& middleware,
        int priority
    ) {
        middlewares.emplace_back(priority, middleware);
        std::ranges::sort(middlewares, [](
            const std::pair<int, std::function<void(CompleteException)>>& first,
            const std::pair<int, std::function<void(CompleteException)>>& second
        ) {
            return first.first > second.first;
        });
    }
} // namespace smart_home::usp_protocol::utilities::exceptions