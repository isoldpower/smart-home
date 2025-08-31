#include <smart_home/daemon/include/Daemon.h>
#include <smart_home/daemon/include/SignalDaemon.h>
#include <smart_home/daemon/include/ExitCodes.h>

#include <iostream>


using namespace smart_home::daemon;

class DelayedSignalDaemon : public SignalDaemon {
    void onBeforeShutdown() override {
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
};

int main() {
    DaemonProcess* daemon = new SignalDaemon();
    daemon->bootstrap();
    while (daemon->getIsActive()) {
        std::cout << "Running..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    daemon->waitForShutdown();
    return 0;
}
