#pragma once

#include "./model/UspServer.h"
#include "./UspServerConfig.h"


// TODO: Replace all exit() states with exceptions including detailed messages and error codes.
namespace smart_home::usp_server {

    class UspSyncServer : public UspServer {
    private:
        char* buffer;
    public:
        explicit UspSyncServer(const UspServerConfig&);
        ~UspSyncServer();

        void startServer();
        void closeServer();
        void receiveMessage();
        void startServerCycle(const std::function<bool()>& = []() { return true; });
    };
} // namespace smart_home::usp_server