#include "../include/DaemonRunner.h"


namespace smart_home::daemon {

    DaemonRunner::DaemonRunner(DaemonProcess* process)
        : underlyingProcess(process)
        , wrappers({})
    {}

    void DaemonRunner::cycleProcedure(const std::function<void()>& iterationAction) const {
        underlyingProcess->bootstrap();

        while (underlyingProcess->getIsActive()) {
            if (wrappers.empty() == true) {
                iterationAction();
            } else {
                for (const auto& wrapper : wrappers) {
                    wrapper->runProcedure(iterationAction);
                }
            }
        }

        underlyingProcess->waitForShutdown();
    }

    void DaemonRunner::addWrapper(ProcedureWrapper* wrapper) {
        wrappers.push_back(wrapper);
    }
} // namespace smart_home::daemon