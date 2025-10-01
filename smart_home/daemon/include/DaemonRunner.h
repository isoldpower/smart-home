#pragma once

#include <functional>

#include "./Daemon.h"


namespace smart_home::daemon {

    class ProcedureWrapper {
    public:
        virtual void runProcedure(const std::function<void()>& function) = 0;
        virtual ~ProcedureWrapper() = default;
    };

    class DaemonRunner {
    private:
        DaemonProcess* underlyingProcess;
        std::vector<ProcedureWrapper*> wrappers;
    public:
        explicit DaemonRunner(DaemonProcess* process);

        void cycleProcedure(const std::function<void()>& iterationAction) const;
        void addWrapper(ProcedureWrapper* wrapper);
    };
} // namespace smart_home::daemon