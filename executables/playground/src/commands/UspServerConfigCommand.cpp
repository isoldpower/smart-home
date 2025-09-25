#include "../../include/commands/UspServerConfigCommand.h"

#include <smart_home/usp_server/include/UspServer.h>
#include <smart_home/usp_server/include/UspServerRequest.h>
#include <smart_home/usp_server/include/UspServerResponse.h>
#include <smart_home/daemon/include/Daemon.h>
#include <smart_home/daemon/include/SignalDaemon.h>
#include <smart_home/utilities/include/TypesHelper.h>

#include <iostream>


namespace smart_home::playground::commands {

    void handleRootEndpoint(
        const usp_server::UspServerRequest& req,
        usp_server::UspServerResponse& res
    ) {
        std::cout << "Handling request to root endpoint..." << std::endl;
    }

    int UspServerConfigCommand::execute() {
        usp_server::UspServer uspServer({
            "localhost",
            12345
        });
        uspServer.setHandler(handleRootEndpoint);
        // uspServer.addMiddleware(std::make_shared<usp_protocol::middlewares::LoggingMiddleware>());

        daemon::DaemonProcess* daemon = new daemon::SignalDaemon();
        daemon->bootstrap();
        uspServer.startServer(utilities::methodToFunction(
            &daemon::DaemonProcess::getIsActive,
            daemon
        ));

        return 0;
    }

    void UspServerConfigCommand::present() const {
        std::cout << "USP Server Configuration: "
                     "Configures the USP server library with complex configuration "
                     "and tests all configuration options..." << std::endl;
    }
} // namespace smart_home::usp_protocol::commands