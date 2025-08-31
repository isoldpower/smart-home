#pragma once

#include <sys/signal.h>
#include <vector>

#include "./Daemon.h"

namespace smart_home::daemon {

    class SignalDaemon : public DaemonProcess {
    private:
        const std::vector<int> signalsHandled;
        bool isRunning = false;
        bool isShuttingDown = false;

        static void handleShutdownSignal(int signal);
        void freeSingletonInstance() const;
        void handleShutdown();
    public:
        explicit SignalDaemon(std::vector<int> signals = { SIGABRT, SIGTERM, SIGINT });
        ~SignalDaemon() override;
        static SignalDaemon* activeInstance;

        void shutdown() override;
        void bootstrap() override;
        [[nodiscard]] static SignalDaemon* getActiveInstance();
        [[nodiscard]] bool getIsRunning() const override;
    };
}