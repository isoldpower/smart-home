#include "../include/UspSyncServer.h"

#include <smart_home/usp_protocol/include/CommonMessageData.h>
#include <smart_home/usp_protocol/include/MessageUnion.h>


namespace smart_home::usp_server {

    UspSyncServer::UspSyncServer(const UspServerConfig& config)
        : UspServer(config)
        , buffer(new char[MessageSettings::MAX_PACKET_SIZE])
    {}

    UspSyncServer::~UspSyncServer() {
        delete[] buffer;
    }

    void UspSyncServer::startServer() {
        netServer.establishConnection();
        std::cout << "USP Sync Server started..." << std::endl;
    }

    void UspSyncServer::closeServer() {
        netServer.closeConnection();
        std::cout << "USP Sync Server stopped." << std::endl;
    }

    void UspSyncServer::receiveMessage() {
        const web_server::NetServerClientInfo client = netServer.receiveMessage(
            MessageSettings::MAX_PACKET_SIZE,
            buffer,
            timeval{ 1, 0 }
        );

        if (client.isSuccessful) {
            handleRequestReceived(buffer, client);
        }
    }

    void UspSyncServer::startServerCycle(const std::function<bool()>& isRunningSignal) {
        startServer();

        while (isRunningSignal()) {
            receiveMessage();
        }

        closeServer();
    }
} // namespace smart_home::usp_server