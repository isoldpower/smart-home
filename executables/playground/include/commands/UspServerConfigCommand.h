#pragma once

#include <smart_home/utilities/include/patterns/PresentableCommand.h>
#include <smart_home/utilities/include/exceptions/ExceptionsLoggerShell.h>
#include <smart_home/daemon/include/DaemonRunner.h>

#include <utility>


namespace smart_home::playground::commands {

    class DaemonExceptionsLoggerShell : public daemon::ProcedureWrapper {
    private:
        utilities::exceptions::ExceptionsLoggerShell loggerShellImpl;
        std::string procedureName;
    public:
        explicit DaemonExceptionsLoggerShell(
            utilities::exceptions::ExceptionsLoggerShell shell = {},
            std::string procedureName = "Anonymous Procedure"
        )
            : loggerShellImpl(std::move(shell))
            , procedureName(std::move(procedureName))
        {}

        void runProcedure(const std::function<void()>& function) override {
            loggerShellImpl.safeRunProcedure(procedureName.c_str(), function);
        }
    };

    class UspServerConfigCommand : public utilities::patterns::PresentableCommand<int> {
    public:
        int execute(int argc, char* argv[]) override;
        void present() const override;
    };
} // namespace smart_home::playground::commands