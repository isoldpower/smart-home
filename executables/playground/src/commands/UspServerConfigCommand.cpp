#include "../../include/commands/UspServerConfigCommand.h"

#include <smart_home/daemon/include/Daemon.h>
#include <smart_home/daemon/include/DaemonRunner.h>
#include <smart_home/daemon/include/SignalDaemon.h>
#include <smart_home/usp_server/include/UspServerRequest.h>
#include <smart_home/usp_server/include/UspServerResponse.h>
#include <smart_home/usp_server/include/UspSyncServer.h>
#include <smart_home/utilities/include/TypesHelper.h>
#include <smart_home/utilities/include/exceptions/ExceptionsLoggerShell.h>
#include <iostream>


namespace smart_home::playground::commands {

    void handleRootEndpoint(
        const usp_server::UspServerRequest&,
        usp_server::UspServerResponse&
    ) {
        std::cout << "Handling request to root endpoint..." << std::endl;
    }

    int UspServerConfigCommand::execute(int, char*[]) {
        usp_server::UspSyncServer uspServer({
            "localhost",
            12345
        });
        uspServer.setHandler(handleRootEndpoint);
        // uspServer.addMiddleware(std::make_shared<usp_protocol::middlewares::LoggingMiddleware>());

        daemon::DaemonProcess* daemon = new daemon::SignalDaemon();
        daemon::DaemonRunner runner(daemon);
        DaemonExceptionsLoggerShell loggerMiddleware({}, "USP Server Daemon Procedure");
        runner.addWrapper(&loggerMiddleware);

        uspServer.startServer();
        runner.cycleProcedure([&uspServer]() {
            uspServer.receiveMessage();
        });
        uspServer.closeServer();

        return 0;
    }

    void UspServerConfigCommand::present() const {
        std::cout << "USP Server Configuration: "
                     "Configures the USP server library with complex configuration "
                     "and tests all configuration options..." << std::endl;
    }
} // namespace smart_home::usp_protocol::commands