#include "../../include/version1/UspAsyncServer.h"

#include <smart_home/usp_protocol/include/version1/MessageBasisHandler.h>

#include "../../include/version1/packets/SequencedPacketPoller.h"
#include "../../include/version1/message_handlers/MessageHandler.h"
#include "../../include/version1/message_handlers/MessageHandlerBuilder.h"


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

        const UspServerClient uspClient{ client };

        if (client.isSuccessful) {
            proceedMessage(buffer, uspClient);
        } else {
            delete[] buffer;
        }
    }

    void UspAsyncServer::proceedMessage(
        const char* buffer,
        const UspServerClient& client
    ) {
        const usp_protocol::version1::MessageBasisHandler basisHandler;
        const usp_protocol::version1::CommonMessagePacketData commonData =
            basisHandler.parseCommonData(buffer, client.bytesReceived);

        message_handlers::MessageHandlerBuilder builder;
        const std::unique_ptr<message_handlers::MessageHandler> handler =
            builder.buildMessageHandler(commonData);
        handler->handleMessage(buffer, client);
    }
} // namespace smart_home::usp_server::version1