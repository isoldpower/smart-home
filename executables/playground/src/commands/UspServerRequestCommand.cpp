#include "../../include/commands/UspServerRequestCommand.h"

#include <arpa/inet.h>
#include <smart_home/usp_protocol/include/server/UspServer.h>
#include <unistd.h>

#include <iostream>
#include <thread>

namespace smart_home::playground::commands {

    void UspServerRequestCommand::openRequestConnection() {
        activeServer.establishConnection();
    }

    void UspServerRequestCommand::receiveRequest() {
        std::array<char, 1024> buffer{};
        std::cout << "Listening on port " << config.port << "..." << std::endl;
        UspServerClientInfo client = activeServer.receiveMessage(buffer.size(), buffer.data());
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

    void UspServerRequestCommand::fakeClientRequest() {
        int sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock < 0) {
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
            sock,
            message,
            strlen(message),
            0,
            reinterpret_cast<sockaddr*>(&serverAddr),
            sizeof(serverAddr)
        );
        if (bytesSent < 0) {
            std::cerr << "Failed to send message: " << strerror(errno) << std::endl;
            close(sock);
            return;
        }
        std::cout << "[CLIENT] Sent: " << message << " to " << config.ip << ":" << config.port << std::endl;

        // Receive response
        char buffer[1024]{};
        sockaddr_in fromAddr{};
        socklen_t fromLen = sizeof(fromAddr);
        ssize_t bytesReceived = recvfrom(
            sock,
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

        close(sock);
    }

    int UspServerRequestCommand::execute() {
        openRequestConnection();
        std::thread serverThread(&UspServerRequestCommand::receiveRequest, this);

        fakeClientRequest();

        serverThread.join();
        return 0;
    }

    void UspServerRequestCommand::present() const {
        std::cout << "USP Network Requests: "
                     "Runs USP server instance and waits for network requests..." << std::endl;
    }
} // namespace smart_home::usp_protocol::commands