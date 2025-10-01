#pragma once

#include <smart_home/utilities/include/patterns/PresentableCommand.h>
#include <smart_home/daemon/include/SignalDaemon.h>


namespace smart_home::playground::commands {
    using namespace smart_home::daemon;

    class DelayedSignalDaemon : public daemon::SignalDaemon {
    protected:
        void onBeforeShutdown() override;
    };

    class DelayedDaemonCommand : public utilities::patterns::PresentableCommand<int> {
    public:
        int execute(int argc, char* argv[]) override;
        void present() const override;
    };
} // namespace smart_home::playground::commands