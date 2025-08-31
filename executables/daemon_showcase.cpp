#include <smart_home/daemon/include/Daemon.h>
#include <smart_home/daemon/include/SignalDaemon.h>

#include <unistd.h>
#include <iostream>


using namespace smart_home::daemon;

int main() {
    DaemonProcess* daemon = new SignalDaemon();
    daemon->bootstrap();
    while (daemon->getIsRunning()) {
        std::cout << "Running" << std::endl;
        sleep(1);
    }

    return 0;
}
