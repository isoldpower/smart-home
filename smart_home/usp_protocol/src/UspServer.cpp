#include "../include/server/UspServer.h"

#include <netinet/in.h>
#include <smart_home/usp_protocol/include/server/ExitCodes.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <ostream>


namespace smart_home::usp_protocol::server {

    UspServer::UspServer(const UspServerConfig &config)
        : serverPort(config.port)
    {}

    void UspServer::establishConnection() {
        if (socketDescriptor && socketDescriptor > 0) {
            std::cerr << "Connection is already established." << std::endl;
            return;
        }

        associateSocketDescriptor();
        bindSocketToPort();
    }

    void UspServer::closeConnection() {
        if (socketDescriptor < 0) {
            std::cerr << "No valid socket to close." << std::endl;
            return;
        }

        if (const int closeStatus = close(socketDescriptor); closeStatus < 0) {
            std::cerr << "Failed to close UDP socket." << std::endl;
            exit(UspServerExitCodes::EXT_ERR_SOCKET_CREATION);
        } else {
            socketDescriptor = -1;
            std::cout << "UDP socket closed successfully with status " << closeStatus << std::endl;
        }
    }

    UspServerClientInfo UspServer::receiveMessage(const size_t bufferSize, char *buffer) const {
        sockaddr_in clientAddress{};
        socklen_t clientLength = sizeof(clientAddress);
        constexpr int flagsMask = 0;

        ssize_t bytesReceived = recvfrom(
            socketDescriptor,
            buffer,
            bufferSize - 1,
            flagsMask,
            reinterpret_cast<sockaddr*>(&clientAddress),
            &clientLength
        );
        if (bytesReceived < 0) {
            std::cerr << "Failed to receive message from UDP connection." << std::endl;
            close(socketDescriptor);
            exit(UspServerExitCodes::EXT_ERR_RECEIVE_MESSAGE);
        }

        buffer[bytesReceived] = '\0';
        return { clientAddress, clientLength };
    }

    bool UspServer::sendMessage(const char *message, UspServerClientInfo client) const {
        constexpr int flagsMask = 0;
        ssize_t bytesSent = sendto(
            socketDescriptor,
            message,
            strlen(message),
            flagsMask,
            reinterpret_cast<sockaddr*>(&client.clientAddress),
            client.clientLength
        );

        if (bytesSent < 0) {
            std::cerr << "Failed to send message to server." << std::endl;
            return false;
        } else {
            return true;
        }
    }

    int UspServer::associateSocketDescriptor() {
        socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);

        if (socketDescriptor < 0) {
            std::cerr << "Failed to create socket." << std::endl;
            exit(UspServerExitCodes::EXT_ERR_SOCKET_CREATION);
        } else {
            return socketDescriptor;
        }
    }

    void UspServer::bindSocketToPort() const {
        sockaddr_in serverAddress{};
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(serverPort);
        serverAddress.sin_addr.s_addr = INADDR_ANY;

        int bindResult = bind(
            socketDescriptor,
            reinterpret_cast<sockaddr *>(&serverAddress),
            sizeof(serverAddress)
        );
        if (bindResult < 0) {
            std::cerr << "Failed to bind socket to listen for UDP connection." << std::endl;
            close(socketDescriptor);
            exit(UspServerExitCodes::EXT_ERR_SOCKET_BINDING);
        }
    }
} // namespace smart_home::usp_protocol::server