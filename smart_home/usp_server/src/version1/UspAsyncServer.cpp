#include "../../include/version1/UspAsyncServer.h"

#include <smart_home/usp_protocol/include/version1/MessageBasisHandler.h>

#include "../../include/exceptions/ReceivedMessageException.h"


namespace smart_home::usp_server::version1 {

    UspAsyncServer::UspAsyncServer(
        const UspServerConfig &config,
        const HandlerFunction &onRequest
    )
        : UspServer(config, onRequest)
    {}

    void UspAsyncServer::sendRequest(
        const UspServerRequest&,
        const web_server::NetServerClientInfo&,
        ResponseReactionFunction
    ) {
        std::cout << "Trying to send request" << std::endl;
    }

    void UspAsyncServer::tryReceiveMessage(
        const timeval& timeout
    ) {
        const auto buffer = new char[usp_protocol::version1::MessageSettings::MAX_PACKET_SIZE];
        const web_server::NetServerClientInfo client = netServer.receiveMessage(
            usp_protocol::version1::MessageSettings::MAX_PACKET_SIZE,
            buffer,
            timeout
        );

        if (client.isSuccessful) {
            proceedMessage(buffer, client.bytesReceived);
        } else {
            delete[] buffer;
        }
    }

    void UspAsyncServer::proceedMessage(
        const char* buffer,
        const size_t length
    ) {
        const usp_protocol::version1::MessageBasisHandler basisHandler;
        const usp_protocol::version1::CommonMessagePacketData commonData = basisHandler.parseCommonData(
            buffer,
            length
        );

        switch (commonData.type) {
            case usp_protocol::version1::MessageType::MESSAGE_ACKNOWLEDGEMENT:
                std::cout << "Message acknowledgement" << std::endl;
                break;
            case usp_protocol::version1::MessageType::MESSAGE_REQUEST:
                std::cout << "Message request" << std::endl;
                break;
            case usp_protocol::version1::MessageType::MESSAGE_RESPONSE:
                std::cout << "Message response" << std::endl;
                break;
            case usp_protocol::version1::MessageType::MESSAGE_PROTOCOL:
                std::cout << "Message protocol" << std::endl;
                break;
            case usp_protocol::version1::MessageType::MESSAGE_UNKNOWN:
                throw exceptions::ReceivedMessageException(
                    exceptions::ExceptionLevel::WARNING,
                    exceptions::ExecutionCodes::UNKNOWN_MESSAGE_TYPE,
                    "Received message with unknown type; skipping processing."
                );
            default:
                throw exceptions::ReceivedMessageException(
                    exceptions::ExceptionLevel::FATAL,
                    exceptions::ExecutionCodes::UNKNOWN_MESSAGE_TYPE,
                    "Received message type that is not defined in internal server handling process"
                );
        }
    }
} // namespace smart_home::usp_server::version1