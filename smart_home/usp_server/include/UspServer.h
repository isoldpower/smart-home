#pragma once

#include <smart_home/daemon/include/DaemonRunner.h>
#include <smart_home/daemon/include/SignalDaemon.h>
#include <smart_home/web_server/include/NetServer.h>
#include <thread>
#include <iostream>

#include "./UspServerClient.h"
#include "./UspServerConfig.h"


namespace smart_home::usp_server {

    template <typename TRequest, typename TResponse>
    class UspServer {
        using HandlerFunction = std::function<void(const TRequest&, TResponse&)>;
        using ResponseReactionFunction = std::function<void(const TResponse&)>;
    private:
        std::thread serverThread;
        std::atomic_bool isShuttingDown{false};
        std::unique_ptr<daemon::DaemonProcess> referencedProcess = nullptr;
        std::unique_ptr<daemon::DaemonRunner> runner = nullptr;

        void serverLifeCycle() {
            netServer.establishConnection();
            runner->cycleProcedure([this]() {
                tryReceiveMessage(config.receiveIterationHang);
            });

            isShuttingDown.store(true);
            netServer.closeConnection();
        }
    protected:
        web_server::NetServer netServer;
        HandlerFunction handler;
        const UspServerConfig config;
    public:
        explicit UspServer(
            const UspServerConfig& config,
            HandlerFunction onRequest
        )
            : referencedProcess(std::make_unique<daemon::SignalDaemon>())
            , runner(std::make_unique<daemon::DaemonRunner>(referencedProcess.get()))
            , netServer(web_server::NetServerConfig{config.host, config.port})
            , handler(std::move(onRequest))
            , config(config)
        {}

        virtual ~UspServer() {
            stopServer();
        }

        virtual void sendRequest(
            const TRequest& request,
            const web_server::NetServerClientInfo& client,
            ResponseReactionFunction onResponse
        ) = 0;
        virtual void tryReceiveMessage(
            const timeval& timeout
        ) = 0;

        void startServer() {
            if (serverThread.joinable()) {
                std::cerr << "Server already running." << std::endl;
                return;
            }

            serverThread = std::thread(&UspServer::serverLifeCycle, this);
        }

        void joinServerThread() {
            if (serverThread.joinable()) {
                serverThread.join();
            } else {
                std::cerr << "Server thread not joinable, skipping" << std::endl;
            }
        }

        void stopServer() {
            if (runner && !isShuttingDown.exchange(true)) {
                runner->stopProcedure();
                joinServerThread();
            }
        }
    };
} // namespace smart_home::usp_server