#include "../../include/commands/NetServerRequestCommand.h"

#include <smart_home/web_server/include/NetServer.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <thread>


namespace smart_home::playground::commands {

    void NetServerRequestCommand::openRequestConnection() {
        activeServer.establishConnection();
    }

    void NetServerRequestCommand::receiveRequest() {
        std::array<char, 4096> buffer{};
        std::cout << "Listening on port " << config.port << "..." << std::endl;
        NetServerClientInfo client = activeServer.receiveMessage(
            buffer.size(),
            buffer.data(),
            timeval{ 5, 0 }
        );
        if constexpr (!buffer.empty()) {
            std::cout << "[SERVER] Received: " << buffer.data() << std::endl;
            // Echo back to client
            std::string response = "Echo: " + std::string(buffer.data());
            if (activeServer.sendMessage(response.c_str(), client)) {
                std::cout << "[SERVER] Sent: " << response << std::endl;
            }
        }

        activeServer.closeConnection();
    }

    void NetServerRequestCommand::fakeClientRequest() const {
        const int clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
        if (clientSocket < 0) {
            std::cerr << "Failed to create socket: " << strerror(errno) << std::endl;
            return;
        }

        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(config.port);
        size_t attemptIndex = 0;
        int connectionResult;
        do {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            connectionResult = inet_pton(AF_INET, config.ip, &serverAddr.sin_addr);
            attemptIndex++;
        } while (attemptIndex < 5 && connectionResult <= 0);

        // Send message
        const char* message = "Hello, world from fake client!";
        ssize_t bytesSent = sendto(
            clientSocket,
            message,
            strlen(message),
            0,
            reinterpret_cast<sockaddr*>(&serverAddr),
            sizeof(serverAddr)
        );
        if (bytesSent < 0) {
            std::cerr << "Failed to send message: " << strerror(errno) << std::endl;
            close(clientSocket);
            return;
        }
        std::cout << "[CLIENT] Sent: " << message << " to " << config.ip << ":" << config.port << std::endl;

        // Receive response
        char buffer[4096]{};
        sockaddr_in fromAddr{};
        socklen_t fromLen = sizeof(fromAddr);
        ssize_t bytesReceived = recvfrom(
            clientSocket,
            buffer,
            sizeof(buffer) - 1,
            0,
            reinterpret_cast<sockaddr*>(&fromAddr),
            &fromLen
        );
        if (bytesReceived < 0) {
            std::cerr << "Failed to receive response: " << strerror(errno) << std::endl;
        } else {
            buffer[bytesReceived] = '\0';
            std::cout << "[CLIENT] Received: " << buffer << " from "
                      << inet_ntoa(fromAddr.sin_addr) << ":"
                      << ntohs(fromAddr.sin_port) << std::endl;
        }

        close(clientSocket);
    }

    int NetServerRequestCommand::execute() {
        openRequestConnection();
        std::thread serverThread(&NetServerRequestCommand::receiveRequest, this);

        fakeClientRequest();

        serverThread.join();
        return 0;
    }

    void NetServerRequestCommand::present() const {
        std::cout << "USP Network Requests: "
                     "Runs USP server instance and waits for network requests..." << std::endl;
    }
} // namespace smart_home::usp_protocol::commands