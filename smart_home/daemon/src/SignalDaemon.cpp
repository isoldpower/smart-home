#include "../include/SignalDaemon.h"

#include <iostream>
#include <ostream>
#include <thread>

#include "../include/ExitCodes.h"


namespace smart_home::daemon {
    SignalDaemon* SignalDaemon::activeInstance = nullptr;

    SignalDaemon::SignalDaemon(std::vector<int> signals)
        : signalsHandled(std::move(signals))
        , isRunning(false)
        , isShuttingDown(false)
        , isForcedShutdown(false)
    {
        if (SignalDaemon::getActiveInstance() != nullptr) {
            std::cerr << "SignalDaemon instance already exists. There can be only one daemon running at a time." << std::endl;
            std::cout << "Reallocating the memory..." << std::endl;
            freeSingletonInstance();
        }

        activeInstance = this;
    }

    SignalDaemon::~SignalDaemon() {
        if (isRunning.load() && !isShuttingDown.load()) {
            SignalDaemon::shutdown();
        }

        freeSingletonInstance();
    }

    void SignalDaemon::shutdown() {
        std::cout << "Daemon received shutdown call" << std::endl;
        if (isShuttingDown.load()) {
            std::cerr << "Force shutdown requested. Escalating priority..." << std::endl;
            isForcedShutdown.store(true);
        } else {
            std::cout << "Shutting down the process gracefully..." << std::endl
                      << "\t↳ Press Ctrl + C again to force quit (not recommended)." << std::endl;
            isShuttingDown.store(true);
            shutdownThread = std::thread(&SignalDaemon::handleShutdown, this);
        }
    }

    void SignalDaemon::bootstrap() {
        isRunning.store(true);
        for (const int signalIterator : signalsHandled) {
            signal(signalIterator, SignalDaemon::handleShutdownSignal);
        }

        std::cout << "Process is running. Press Ctrl + C to quit." << std::endl;
    }

    void SignalDaemon::waitForShutdown() {
        if (!isShuttingDown.load()) {
            std::cerr << "Daemon is not shutting down. Cannot wait for shutdown." << std::endl;
            return;
        }

        if (shutdownThread.joinable()) {
            shutdownThread.join();
        } else {
            std::cerr << "Shutdown thread is not joinable. Exiting now." << std::endl;
            std::exit(DaemonExitCodes::EXT_ERR_SHUTDOWN_INTERRUPTED);
        }
    }

    SignalDaemon* SignalDaemon::getActiveInstance() {
        return activeInstance;
    }

    bool SignalDaemon::getIsRunning() const {
        return isRunning.load();
    }

    bool SignalDaemon::getIsActive() const {
        const bool isActiveInstance = this == SignalDaemon::getActiveInstance();
        const bool isNotShuttingDown = !isShuttingDown.load();
        const bool isCurrentlyRunning = isRunning.load();

        return isActiveInstance && isNotShuttingDown && isCurrentlyRunning;
    }

    void SignalDaemon::handleShutdownSignal(int signalCode) {
        std::cout << "Received shutdown signal: " << signalCode << std::endl;
        if (SignalDaemon* runningDaemon = SignalDaemon::getActiveInstance()) {
            runningDaemon->shutdown();
        } else {
            std::cerr << "No active SignalDaemon instance found. Exiting now." << std::endl;
            std::exit(DaemonExitCodes::EXT_ERR_DAEMON_NULLPTR);
        }
    }

    void SignalDaemon::freeSingletonInstance() const {
        if (SignalDaemon* runningDaemon = SignalDaemon::getActiveInstance()) {
            if (runningDaemon->isShuttingDown.load()) {
                std::cerr << "Cannot free the singleton instance while it's shutting down." << std::endl;
                std::cerr << "Escalating to force-quit and waiting for shutdown to complete..." << std::endl;
                runningDaemon->isForcedShutdown.store(true);
                runningDaemon->waitForShutdown();
            } else if (runningDaemon->isRunning.load() && runningDaemon != this) {
                runningDaemon->shutdown();
            }

            std::free(runningDaemon);
            activeInstance = nullptr;
        }
    }

    void SignalDaemon::handleShutdown() {
        int seconds = 5;
        std::cout << "1) Simulating high workload during shutdown (5 seconds)..." << std::endl;
        for (int i = 0; i < seconds; i++) {
            if (isForcedShutdown.load()) {
                std::cerr << "Forced shutdown handled. Exiting now." << std::endl;
                std::exit(DaemonExitCodes::EXT_ERR_SHUTDOWN_FORCED);
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        std::cout << "2) Falling back to default signal handlers..." << std::endl;
        for (const int signalIterator : signalsHandled) {
            signal(signalIterator, SIG_DFL);
        }

        std::cout << "3) Updating instance state to idle" << std::endl;
        isRunning.store(false);
        std::cout << "Cleanup completed. Exiting now." << std::endl;
    }
}