#pragma once

#include <sys/signal.h>
#include <thread>
#include <vector>

#include "./Daemon.h"

namespace smart_home::daemon {

    class SignalDaemon : public DaemonProcess {
    private:
        const std::vector<int> signalsHandled;
        std::thread shutdownThread;
        static void handleShutdownSignal(int signal);
        void freeSingletonInstance() const;
        void handleShutdown();
    protected:
        std::atomic<bool> isRunning{false};
        std::atomic<bool> isShuttingDown{false};
        std::atomic<bool> isForcedShutdown{false};
    public:
        explicit SignalDaemon(std::vector<int> signals = { SIGABRT, SIGTERM, SIGINT });
        ~SignalDaemon() override;
        static SignalDaemon* activeInstance;

        void shutdown() override;
        void bootstrap() override;
        void waitForShutdown() override;
        [[nodiscard]] static SignalDaemon* getActiveInstance();
        [[nodiscard]] bool getIsRunning() const;
        [[nodiscard]] bool getIsActive() const override;

        virtual void onBeforeShutdown() {}
        virtual void onAfterShutdown() {}
    };
}