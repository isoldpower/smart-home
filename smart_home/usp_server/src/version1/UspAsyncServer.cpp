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
        , commonPacketPoller(std::make_unique<packets::SequencedPacketPoller<
            uint16_t,
            ReferencedCommonData
        >>())
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
        usp_protocol::version1::CommonMessagePacketData commonData =
            basisHandler.parseCommonData(buffer, client.bytesReceived);
        commonPacketPoller->addPacket(
            commonData.requestId,
            std::make_shared<ReferencedCommonData>(commonData, buffer, client)
        );

        if (commonPacketPoller->isSequenceComplete(commonData.requestId)) {
            std::vector<std::shared_ptr<ReferencedCommonData>> packets =
                commonPacketPoller->getAllPackets(commonData.requestId);
            std::vector<char*> packetBuffers;
            packetBuffers.resize(commonData.packetsCount);
            std::ranges::transform(
                packets,
                std::back_inserter(packetBuffers),
                [](const std::shared_ptr<ReferencedCommonData>& packet) {
                    return packet->getRawDataReference();
                }
            );

            // Message can be considered completely received at this point.
            // Need to properly process it. ACKs, Request Polling, connection establishment.
            message_handlers::MessageHandlerBuilder builder;
            const std::unique_ptr<message_handlers::MessageHandler> handler =
                builder.buildMessageHandler(commonData);

            handler->handleMessage(packets);
        }
    }
} // namespace smart_home::usp_server::version1