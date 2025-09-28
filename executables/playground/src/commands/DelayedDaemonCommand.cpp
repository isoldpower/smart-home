#include "../../include/commands/DelayedDaemonCommand.h"

#include <smart_home/daemon/include/Daemon.h>
#include <smart_home/daemon/include/ExitCodes.h>
#include <smart_home/daemon/include/SignalDaemon.h>
#include <iostream>


namespace smart_home::playground::commands {

    void DelayedSignalDaemon::onBeforeShutdown() {
        int seconds = 5;
        std::cout << "- Simulating high workload during shutdown (5 seconds)..." << std::endl;
        for (int i = 0; i < seconds; i++) {
            if (isForcedShutdown.load()) {
                std::cerr << "Forced shutdown handled. Exiting now." << std::endl;
                std::exit(DaemonExitCodes::EXT_ERR_SHUTDOWN_FORCED);
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    int DelayedDaemonCommand::execute(int argc, char* argv[]) {
        DaemonProcess* daemon = new DelayedSignalDaemon();
        daemon->bootstrap();
        while (daemon->getIsActive()) {
            std::cout << "Running..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        daemon->waitForShutdown();
        return 0;
    }

    void DelayedDaemonCommand::present() const {
        std::cout << "Delayed Daemon Command: "
                     "Demonstrates a simple daemon that simulates a "
                     "delay during shutdown." << std::endl;
    }
} // namespace smart_home::playground
