#pragma once

#include <smart_home/utilities/include/patterns/PresentableCommand.h>
#include <smart_home/web_server/include/NetServer.h>
#include <smart_home/web_server/include/NetServerConfig.h>


namespace smart_home::playground::commands {
    using namespace smart_home::web_server;

    class NetServerRequestCommand : public utilities::patterns::PresentableCommand<int> {
    private:
        NetServerConfig config{};
        NetServer activeServer;

        void openRequestConnection();
        void fakeClientRequest() const;
        void receiveRequest();
    public:
        NetServerRequestCommand()
            : config(NetServerConfig{ "localhost", 12345 })
            , activeServer(config)
        {}
        int execute(int argc, char* argv[]) override;
        void present() const override;
    };
} // namespace smart_home::usp_protocol::commands