#include "../include/NetServer.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <ostream>

#include "../include/ExitCodes.h"

namespace smart_home::web_server {

    NetServer::NetServer(const NetServerConfig &config)
        : serverPort(config.port)
    {}

    void NetServer::establishConnection() {
        if (socketDescriptor && socketDescriptor > 0) {
            std::cerr << "Connection is already established." << std::endl;
            return;
        }

        associateSocketDescriptor();
        bindSocketToPort();
    }

    void NetServer::closeConnection() {
        if (socketDescriptor < 0) {
            std::cerr << "No valid socket to close." << std::endl;
            return;
        }

        if (const int closeStatus = close(socketDescriptor); closeStatus < 0) {
            std::cerr << "Failed to close UDP socket." << std::endl;
            exitWithCode(NetServerExitCodes::EXT_ERR_SOCKET_CREATION);
        } else {
            socketDescriptor = -1;
            std::cout << "UDP socket closed successfully with status " << closeStatus << std::endl;
        }
    }

    NetServerClientInfo NetServer::receiveMessage(
        const size_t bufferSize,
        char *buffer,
        const timeval& timeout
    ) const {
        sockaddr_in clientAddress{};
        socklen_t clientLength = sizeof(clientAddress);

        if (const bool isPackageReceived = setListenTimeout(timeout); !isPackageReceived) {
            buffer[0] = '\0';
            return { clientAddress, clientLength, 0, false };
        } else {
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
                exitWithCode(NetServerExitCodes::EXT_ERR_RECEIVE_MESSAGE);
            }

            buffer[bytesReceived] = '\0';
            return { clientAddress, clientLength, bytesReceived, true };
        }
    }

    bool NetServer::sendMessage(const char *message, NetServerClientInfo client) const {
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

    int NetServer::associateSocketDescriptor() {
        socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);

        if (socketDescriptor < 0) {
            std::cerr << "Failed to create socket." << std::endl;
            exitWithCode(NetServerExitCodes::EXT_ERR_SOCKET_CREATION);
        } else {
            return socketDescriptor;
        }
    }

    void NetServer::bindSocketToPort() const {
        sockaddr_in serverAddress{};
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(serverPort);
        // TODO: We can add the 'allowed host' feature here later.
        serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

        int bindResult = bind(
            socketDescriptor,
            reinterpret_cast<sockaddr *>(&serverAddress),
            sizeof(serverAddress)
        );
        if (bindResult < 0) {
            std::cerr << "Failed to bind socket to listen for UDP connection." << std::endl;
            close(socketDescriptor);
            exitWithCode(NetServerExitCodes::EXT_ERR_SOCKET_BINDING);
        }
    }

    bool NetServer::setListenTimeout(const timeval &timeout) const {
        fd_set readFds;
        FD_ZERO(&readFds);
        FD_SET(socketDescriptor, &readFds);

        timeval timeoutCopy = timeout;
        int selectResult = select(
            socketDescriptor + 1,
            &readFds,
            nullptr,
            nullptr,
            &timeoutCopy
        );

        return selectResult > 0;
    }
} // namespace smart_home::usp_protocol::net