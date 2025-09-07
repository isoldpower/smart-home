#pragma once

#include <smart_home/utilities/include/patterns/PresentableCommand.h>

#include "server/UspServer.h"
#include "server/UspServerConfig.h"

namespace smart_home::playground::commands {
    using namespace usp_protocol::server;

    class UspServerRequestCommand : public utilities::patterns::PresentableCommand<int> {
    private:
        UspServerConfig config{};
        UspServer activeServer;

        void openRequestConnection();
        void fakeClientRequest();
        void receiveRequest();
    public:
        UspServerRequestCommand()
            : config({ "localhost", 12345 })
            , activeServer(config)
        {}
        int execute() override;
        void present() const override;
    };
} // namespace smart_home::usp_protocol::commands