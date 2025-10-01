#pragma once

#include <smart_home/utilities/include/patterns/PresentableCommand.h>
#include <netinet/in.h>
#include <string>
#include <memory>


namespace smart_home::playground::commands {

    struct CliArgs {
    public:
        int port;
        const char* ip;

        CliArgs(int argc, char* argv[]);
    };

    class UspClientRequestCommand : public utilities::patterns::PresentableCommand<int> {
    private:
        int createSocket();
        std::unique_ptr<sockaddr_in> openServerConnection(CliArgs args);
        void sendRequest(int clientSocket, sockaddr_in address, const std::string& message);
        void sendAcknowledgementMessage(
            const std::string& message,
            int clientSocket,
            const sockaddr_in* serverAddress
        );
        void sendRequestMessage(
            const std::string& message,
            int clientSocket,
            const sockaddr_in* serverAddress
        );
        void sendResponseMessage(
            const std::string& message,
            int clientSocket,
            const sockaddr_in* serverAddress
        );
        void sendProtocolMessage(
            const std::string& message,
            int clientSocket,
            const sockaddr_in* serverAddress
        );
    public:
        void present() const override;
        int execute(int argc, char* argv[]) override;
    };
} // namespace smart_home::playground::commands