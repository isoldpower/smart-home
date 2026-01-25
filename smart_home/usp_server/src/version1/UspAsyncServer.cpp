#include "../../include/version1/UspAsyncServer.h"

#include <smart_home/usp_protocol/include/version1/MessageBasisHandler.h>
#include <smart_home/usp_server/include/version1/events/MessageReceivedEvent.h>

#include "../../include/version1/packets/SequencedPacketPoller.h"
#include "version1/request/RequestMessageHandler.h"


// TODO: Include message type to the packet poller as it can erase conflicts. For example:
// ACK messages have the same RequestID as REQUEST messages do. It will packet poll twice.
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
        , eventChannel(std::make_shared<utilities::patterns::EventChannel>())
    {
        requestHandler = std::make_unique<message_handlers::RequestHandler>(eventChannel);
        responseHandler = std::make_unique<message_handlers::ResponseHandler>(eventChannel);
        acknowledgmentHandler = std::make_unique<message_handlers::AcknowledgementHandler>(eventChannel);
        protocolHandler = std::make_unique<message_handlers::ProtocolHandler>(eventChannel);
    }

    void UspAsyncServer::sendRequest(
        const UspServerRequest& request,
        const web_server::NetServerClientInfo& client,
        ResponseReactionFunction callback
    ) {
        usp_protocol::version1::RequestMessageHandler handler;
        const size_t packetsCount = ceil(
            request.data.size() / usp_protocol::version1::MessageSettings::MAX_PACKET_SIZE
        );

        for (size_t i = 0; i < packetsCount; ++i) {
            const std::string packetChunk = request.data.substr(
                i * usp_protocol::version1::MessageSettings::MAX_PACKET_SIZE,
                usp_protocol::version1::MessageSettings::MAX_PACKET_SIZE
            );
            usp_protocol::version1::RequestMessage packetData {
                request.protocolVersion,
                request.sessionId,
                request.timestamp,
                request.requestId,
                i,
                packetsCount,
                request.auth,
                request.actionGroup,
                request.action,
                request.size,
                request.data
            };

            const std::unique_ptr<
                usp_protocol::version1::RequestSerializationResult
            > packet = handler.serialize(&packetData);
            if (packet->getIsSuccess()) {
                const std::string messageRaw{
                    packet->getSerializationState()->begin(),
                    packet->getSerializationState()->end()
                };
                if (netServer.sendMessage(messageRaw.data(), client)) {
                    std::cout << "Message Sent" << std::endl;
                }
            }
        }
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
        usp_protocol::version1::CommonMessagePacketData commonData = basisHandler.parseCommonData(
            buffer,
            client.bytesReceived
        );
        commonPacketPoller->addPacket(
            commonData.requestId,
            std::make_shared<ReferencedCommonData>(commonData, buffer, client)
        );

        if (commonPacketPoller->isSequenceComplete(commonData.requestId)) {
            processCompleteMessage(commonData);
            commonPacketPoller->freeSequence(commonData.requestId);
        }
    }

    void UspAsyncServer::processCompleteMessage(
        const usp_protocol::version1::CommonMessagePacketData& referencePacket
    ) const {
        using PacketReference = std::shared_ptr<ReferencedCommonData>;
        using ListOfPackets = std::vector<PacketReference>;

        ListOfPackets packets = commonPacketPoller->getAllPackets(referencePacket.requestId);
        std::vector<char*> packetBuffers;
        packetBuffers.resize(referencePacket.packetsCount);
        std::ranges::transform(
            packets,
            std::back_inserter(packetBuffers),
            [](const PacketReference& packet) {
                return packet->getRawDataReference();
            }
        );

        // Message can be considered completely received at this point.
        // Need to properly process it. ACKs, Request Polling, connection establishment.
        eventChannel->dispatch(events::MessageReceivedEvent(packets));
    }
} // namespace smart_home::usp_server::version1