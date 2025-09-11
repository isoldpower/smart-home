#include "../../include/commands/UspServerConfigCommand.h"

#include <smart_home/usp_protocol/include/UspServer.h>
#include <smart_home/usp_protocol/include/UspServerRequest.h>
#include <smart_home/usp_protocol/include/UspServerResponse.h>

#include <iostream>

#include "Daemon.h"
#include "SignalDaemon.h"
#include "TypesHelper.h"

namespace smart_home::playground::commands {

    void handleRootEndpoint(
        const usp_protocol::UspServerRequest& req,
        usp_protocol::UspServerResponse& res
    ) {
        std::cout << "Handling request to root endpoint..." << std::endl;
    }

    int UspServerConfigCommand::execute() {
        usp_protocol::UspServer uspServer({
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