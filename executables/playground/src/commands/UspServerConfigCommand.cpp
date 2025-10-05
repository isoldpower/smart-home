#include "../../include/commands/UspServerConfigCommand.h"

#include <smart_home/usp_server/include/version1/UspServerRequest.h>
#include <smart_home/usp_server/include/version1/UspServerResponse.h>
#include <smart_home/usp_server/include/version1/UspAsyncServer.h>
#include <smart_home/utilities/include/TypesHelper.h>

#include <iostream>


namespace smart_home::playground::commands {

    void handleRootEndpoint(
        const usp_server::UspServerRequest&,
        usp_server::UspServerResponse&
    ) {
        std::cout << "Handling request to root endpoint..." << std::endl;
    }

    int UspServerConfigCommand::execute(int, char*[]) {
        usp_server::UspAsyncServer uspServer(
            { "localhost", 12345 },
            handleRootEndpoint
        );
        // uspServer.addMiddleware(std::make_shared<usp_protocol::middlewares::LoggingMiddleware>());

        uspServer.startServer();
        uspServer.joinServerThread();

        return 0;
    }

    void UspServerConfigCommand::present() const {
        std::cout << "USP Server Configuration: "
                     "Configures the USP server library with complex configuration "
                     "and tests all configuration options..." << std::endl;
    }
} // namespace smart_home::usp_protocol::commands