#include <smart_home/daemon/include/Daemon.h>
#include <smart_home/daemon/include/SignalDaemon.h>

#include <unistd.h>
#include <iostream>


using namespace smart_home::daemon;

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
