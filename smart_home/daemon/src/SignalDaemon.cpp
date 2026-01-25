#include "../include/SignalDaemon.h"

#include <iostream>
#include <ostream>
#include <thread>

#include "../include/ExitCodes.h"


namespace smart_home::daemon {

    SignalDaemon::SignalDaemon(std::vector<int> signals)
        : signalsHandled(std::move(signals))
        , isRunning(false)
        , isShuttingDown(false)
        , isForcedShutdown(false)
    {
        if (Singleton::getInstance() != nullptr) {
            std::cerr << "SignalDaemon instance already exists. There can be only one daemon running at a time." << std::endl;
            std::cout << "Reallocating the memory..." << std::endl;
            freeSingletonInstance();
        }

        signalsHandled.shrink_to_fit();
        Singleton::setInstance(this);
    }

    SignalDaemon::~SignalDaemon() {
        if (isRunning.load() && !isShuttingDown.load()) {
            SignalDaemon::shutdown();
        }

        if (shutdownThread.joinable() && !isForcedShutdown.load()) {
            shutdownThread.join();
        }

        freeSingletonInstance();
    }

    void SignalDaemon::bootstrap() {
        isRunning.store(true);
        for (const int signalIterator : signalsHandled) {
            signal(signalIterator, SignalDaemon::handleShutdownSignal);
        }

        std::cout << "Process is running. Press Ctrl + C to quit." << std::endl;
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

    void SignalDaemon::waitForShutdown() {
        if (!isShuttingDown.load() && !shutdownThread.joinable()) {
            std::cerr << "Daemon is not shutting down. Cannot wait for shutdown." << std::endl;
            return;
        } else if (shutdownThread.joinable()) {
            shutdownThread.join();
        } else {
            std::cerr << "Shutdown thread is not joinable. Exiting now." << std::endl;
            std::exit(DaemonExitCodes::EXT_ERR_SHUTDOWN_INTERRUPTED);
        }
    }

    bool SignalDaemon::getIsActive() const {
        const bool isActiveInstance = this == Singleton::getInstance();
        const bool isNotShuttingDown = !isShuttingDown.load();
        const bool isCurrentlyRunning = isRunning.load();

        return isActiveInstance && isNotShuttingDown && isCurrentlyRunning;
    }

    void SignalDaemon::handleShutdownSignal(int signalCode) {
        std::cout << "Received shutdown signal: " << signalCode << std::endl;
        if (SignalDaemon* runningDaemon = Singleton::getInstance()) {
            runningDaemon->shutdown();
        } else {
            std::cerr << "No active SignalDaemon instance found. Exiting now." << std::endl;
            std::exit(DaemonExitCodes::EXT_ERR_DAEMON_NULLPTR);
        }
    }

    void SignalDaemon::freeSingletonInstance() const {
        if (SignalDaemon* runningDaemon = Singleton::getInstance()) {
            if (runningDaemon->isShuttingDown.load()) {
                std::cerr << "Cannot free the singleton instance while it's shutting down." << std::endl;
                std::cerr << "Escalating to force-quit and waiting for shutdown to complete..." << std::endl;
                runningDaemon->isForcedShutdown.store(true);
                runningDaemon->waitForShutdown();
            } else if (runningDaemon->isRunning.load() && runningDaemon != this) {
                runningDaemon->shutdown();
            }

            Singleton::setInstance(nullptr);
        }
    }

    void SignalDaemon::handleShutdown() {
        onBeforeShutdown();

        std::cout << "- Falling back to default signal handlers..." << std::endl;
        for (const int signalIterator : signalsHandled) {
            signal(signalIterator, SIG_DFL);
        }

        std::cout << "- Updating instance state to idle" << std::endl;
        isRunning.store(false);
        isForcedShutdown.store(false);
        isShuttingDown.store(false);
        std::cout << "Cleanup completed. Exiting now." << std::endl;

        onAfterShutdown();
    }
}