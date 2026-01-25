#include "../../include/commands/UspClientRequestCommand.h"

#include <smart_home/usp_protocol/include/version1/acknowledgement/AcknowledgementMessage.h>
#include <smart_home/usp_protocol/include/version1/acknowledgement/AcknowledgementMessageHandler.h>
#include <smart_home/usp_protocol/include/version1/protocol/ProtocolMessage.h>
#include <smart_home/usp_protocol/include/version1/protocol/ProtocolMessageHandler.h>
#include <smart_home/usp_protocol/include/version1/request/RequestMessage.h>
#include <smart_home/usp_protocol/include/version1/request/RequestMessageHandler.h>
#include <smart_home/usp_protocol/include/version1/response/ResponseMessage.h>
#include <smart_home/usp_protocol/include/version1/response/ResponseMessageHandler.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <cstring>


namespace smart_home::playground::commands {

    CliArgs::CliArgs(int argc, char *argv[]) {
        if (argc != 3) {
            std::cerr << "Usage: " << argv[0] << " <IP> <port>" << std::endl;
            throw std::runtime_error("Wrong number of arguments");
        }

        port = htons(std::atoi(argv[2]));
        ip = argv[1];
    }

    void UspClientRequestCommand::present() const {
        std::cout << "USP Client Request: "
                     "Sends a USP request to a specified IP and port via UDP." << std::endl;
    }

    int UspClientRequestCommand::execute(int argc, char* argv[]) {
        int clientSocket = -1;
        try {
            const CliArgs args(argc, argv);
            clientSocket = createSocket();

            const std::unique_ptr<sockaddr_in> serverAddress = openServerConnection(args);
            sendAcknowledgementMessage("Acknowledgement", clientSocket, serverAddress.get());
            sendProtocolMessage("Protocol", clientSocket, serverAddress.get());
            sendRequestMessage("Request", clientSocket, serverAddress.get());
            sendResponseMessage("Response", clientSocket, serverAddress.get());

            return 0;
        } catch (const std::exception& e) {
            if (clientSocket >= 0) {
                close(clientSocket);
            }

            std::cerr << e.what() << std::endl;
            return 1;
        }
    }

    void UspClientRequestCommand::sendAcknowledgementMessage(
        const std::string& message,
        const int clientSocket,
        const sockaddr_in* serverAddress
    ) {
        constexpr size_t packetsCount = 3;
        for (size_t i = 0; i < packetsCount; ++i) {
            usp_protocol::version1::AcknowledgementMessage acknowledgeMessage{
                usp_protocol::ProtocolVersion::VERSION_1,
                322,
                static_cast<uint64_t>(time(nullptr)),
                12301,
                i,
                packetsCount,
                usp_protocol::version1::AcknowledgementStatus::ACKNOWLEDGEMENT_SUCCESS,
                message.size() + 1,
                message + std::to_string(i)
            };
            usp_protocol::version1::AcknowledgementMessageHandler handler;
            const std::unique_ptr<
                usp_protocol::version1::AcknowledgementSerializationResult
            > packet = handler.serialize(&acknowledgeMessage);

            if (packet->getIsSuccess()) {
                std::string messageRaw{
                    packet->getSerializationState()->begin(),
                    packet->getSerializationState()->end()
                };
                sendRequest(clientSocket, *serverAddress, messageRaw);
            }
        }
    }

    void UspClientRequestCommand::sendProtocolMessage(
        const std::string& message,
        const int clientSocket,
        const sockaddr_in* serverAddress
    ) {
        constexpr size_t packetsCount = 3;
        for (size_t i = 0; i < packetsCount; ++i) {
            usp_protocol::version1::ProtocolMessage protocolMessage{
                usp_protocol::ProtocolVersion::VERSION_1,
                322,
                static_cast<uint64_t>(time(nullptr)),
                12302,
                i,
                packetsCount,
                usp_protocol::version1::ProtocolAction::ACTION_HEARTBEAT,
                message.size() + 1,
                message + std::to_string(i)
            };

            usp_protocol::version1::ProtocolMessageHandler handler;
            const std::unique_ptr<
                usp_protocol::version1::ProtocolSerializationResult
            > packet = handler.serialize(&protocolMessage);

            if (packet->getIsSuccess()) {
                const std::string messageRaw{
                    packet->getSerializationState()->begin(),
                    packet->getSerializationState()->end()
                };
                sendRequest(clientSocket, *serverAddress, messageRaw);
            }
        }
    }

    void UspClientRequestCommand::sendRequestMessage(
        const std::string& message,
        const int clientSocket,
        const sockaddr_in* serverAddress
    ) {
        constexpr size_t packetsCount = 3;
        const std::string authEmpty(4, 'A');
        for (size_t i = 0; i < packetsCount; ++i) {
            usp_protocol::version1::RequestMessage requestMessage{
                usp_protocol::ProtocolVersion::VERSION_1,
                322,
                static_cast<uint64_t>(time(nullptr)),
                12303,
                i,
                packetsCount,
                authEmpty,
                0x01,
                0x01,
                message.size() + 1,
                message + std::to_string(i)
            };

            usp_protocol::version1::RequestMessageHandler handler;
            const std::unique_ptr<
                usp_protocol::version1::RequestSerializationResult
            > packet = handler.serialize(&requestMessage);

            if (packet->getIsSuccess()) {
                const std::string messageRaw{
                    packet->getSerializationState()->begin(),
                    packet->getSerializationState()->end()
                };
                sendRequest(clientSocket, *serverAddress, messageRaw);
            }
        }
    }

    void UspClientRequestCommand::sendResponseMessage(
        const std::string& message,
        const int clientSocket,
        const sockaddr_in* serverAddress
    ) {
        constexpr size_t packetsCount = 3;
        for (size_t i = 0; i < packetsCount; ++i) {
            usp_protocol::version1::ResponseMessage responseMessage{
                usp_protocol::ProtocolVersion::VERSION_1,
                322,
                static_cast<uint64_t>(time(nullptr)),
                12304,
                i,
                packetsCount,
                usp_protocol::version1::ResponseStatus::STATUS_OK,
                message.size() + 1,
                message + std::to_string(i)
            };

            usp_protocol::version1::ResponseMessageHandler handler;
            const std::unique_ptr<
                usp_protocol::version1::ResponseSerializationResult
            > packet = handler.serialize(&responseMessage);

            if (packet->getIsSuccess()) {
                std::string messageRaw{
                    packet->getSerializationState()->begin(),
                    packet->getSerializationState()->end()
                };
                sendRequest(clientSocket, *serverAddress, messageRaw);
            }
        }
    }

    int UspClientRequestCommand::createSocket() {
        if (const int sock = socket(AF_INET, SOCK_DGRAM, 0); sock < 0) {
            std::cerr << "Failed to create socket: " << strerror(errno) << std::endl;
            throw std::runtime_error("Socket creation failed");
        } else {
            return sock;
        }
    }

    std::unique_ptr<sockaddr_in> UspClientRequestCommand::openServerConnection(
        const CliArgs args
    ) {
        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = args.port;
        if (inet_pton(AF_INET, args.ip, &serverAddr.sin_addr) <= 0) {
            std::cerr << "Invalid address: " << strerror(errno) << std::endl;
            throw std::runtime_error("Invalid address");
        }

        return std::make_unique<sockaddr_in>(serverAddr);
    }

    void UspClientRequestCommand::sendRequest(
        const int clientSocket,
        sockaddr_in address,
        const std::string &message
    ) {
        const ssize_t bytesSent = sendto(
            clientSocket,
            message.c_str(),
            message.length(),
            0,
            reinterpret_cast<sockaddr*>(&address),
            sizeof(address)
        );

        if (bytesSent < 0) {
            throw std::runtime_error("Failed to send request to server");
        } else {
            std::cout << "Sent: " << message << " to server successfully!" << std::endl;
        }
    }
} // namespace smart_home::playground::commands