#include "../include/SignalDaemon.h"

#include <unistd.h>

#include <iostream>
#include <ostream>

#include "../include/ExitCodes.h"


namespace smart_home::daemon {
    SignalDaemon* SignalDaemon::activeInstance = nullptr;

    SignalDaemon::SignalDaemon(std::vector<int> signals)
        : signalsHandled(std::move(signals))
        , isRunning(false)
        , isShuttingDown(false)
    {
        if (SignalDaemon::getActiveInstance() != nullptr) {
            std::cerr << "SignalDaemon instance already exists. There can be only one daemon running at a time." << std::endl;
            std::cout << "Reallocating the memory..." << std::endl;
            freeSingletonInstance();
        }

        activeInstance = this;
    }

    SignalDaemon::~SignalDaemon() {
        if (isRunning && !isShuttingDown) {
            SignalDaemon::shutdown();
        }

        freeSingletonInstance();
    }

    void SignalDaemon::shutdown() {
        std::cout << "Daemon received shutdown call" << std::endl;
        if (isShuttingDown) {
            std::cout << "Forcing shutdown..." << std::endl;
            std::free(this);
            std::exit(DaemonExitCodes::EXT_ERR_SHUTDOWN_INTERRUPTED);
        }

        std::cout << "Shutting down the process..." << std::endl
                  << "\tPress Ctrl + C again to force quit (not recommended)." << std::endl;
        isShuttingDown = true;
        handleShutdown();
    }

    void SignalDaemon::bootstrap() {
        isRunning = true;
        for (size_t i = 0; i < signalsHandled.size(); i++) {
            signal(signalsHandled[i], SignalDaemon::handleShutdownSignal);
        }

        std::cout << "Process is running. Press Ctrl + C to quit." << std::endl;
    }

    SignalDaemon* SignalDaemon::getActiveInstance() {
        return activeInstance;
    }

    bool SignalDaemon::getIsRunning() const {
        return isRunning;
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
            if (runningDaemon->isRunning && runningDaemon != this) {
                runningDaemon->shutdown();
            }

            std::free(runningDaemon);
            activeInstance = nullptr;
        }
    }

    void SignalDaemon::handleShutdown() {
        isRunning = false;
        sleep(5);

        for (size_t i = 0; i < signalsHandled.size(); i++) {
            signal(signalsHandled[i], SIG_DFL);
        }
        std::cout << "Cleanup completed. Exiting now." << std::endl;
    }
}