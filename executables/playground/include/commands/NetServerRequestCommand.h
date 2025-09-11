#pragma once

#include <smart_home/utilities/include/patterns/PresentableCommand.h>

#include "net/NetServer.h"
#include "net/NetServerConfig.h"

namespace smart_home::playground::commands {
    using namespace usp_protocol;
    using namespace usp_protocol::net;

    class NetServerRequestCommand : public utilities::patterns::PresentableCommand<int> {
    private:
        NetServerConfig config{};
        NetServer activeServer;

        void openRequestConnection();
        void fakeClientRequest() const;
        void receiveRequest();
    public:
        NetServerRequestCommand()
            : config({ "localhost", 12345 })
            , activeServer(config)
        {}
        int execute() override;
        void present() const override;
    };
} // namespace smart_home::usp_protocol::commands