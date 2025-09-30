#pragma once

#include <thread>

#include "./UspServerConfig.h"
#include "./UspServerRequest.h"
#include "./model/UspServer.h"


// TODO: Replace all exit() states with exceptions including detailed messages and error codes.
namespace smart_home::usp_server {

    class UspAsyncServer : public UspServer {
    private:
        std::thread serverThread;
        std::atomic<bool> isRunning;
    public:
        explicit UspAsyncServer(const UspServerConfig&);

        void bootstrapServer();
        void shutdownServer();
    };
} // namespace smart_home::usp_server