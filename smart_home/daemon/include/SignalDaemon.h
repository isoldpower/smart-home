#pragma once

#include <../../utilities/include/patterns/Singleton.h>
#include <sys/signal.h>

#include <thread>
#include <vector>

#include "./Daemon.h"

namespace smart_home::daemon {

    class SignalDaemon
        : public DaemonProcess
        , public utilities::patterns::Singleton<SignalDaemon>
    {
    private:
        std::vector<int> signalsHandled;
        std::thread shutdownThread;

        static void handleShutdownSignal(int signal);
        void freeSingletonInstance() const;
        void handleShutdown();
    protected:
        std::atomic<bool> isRunning{false};
        std::atomic<bool> isShuttingDown{false};
        std::atomic<bool> isForcedShutdown{false};

        // Additional hooks for subclasses
        virtual void onBeforeShutdown() {}
        virtual void onAfterShutdown() {}
    public:
        explicit SignalDaemon(std::vector<int> signals = { SIGABRT, SIGTERM, SIGINT });
        ~SignalDaemon() override;

        void shutdown() override;
        void bootstrap() override;
        void waitForShutdown() override;
        [[nodiscard]] bool getIsActive() const override;
    };
}