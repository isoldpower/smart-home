#pragma once

namespace smart_home::daemon {
    class DaemonProcess {
    public:
        virtual ~DaemonProcess() = default;
        virtual void shutdown() = 0;
        virtual void bootstrap() = 0;

        virtual void waitForShutdown() = 0;
        [[nodiscard]] virtual bool getIsActive() const = 0;
    };
} // namespace smart_home::daemon
