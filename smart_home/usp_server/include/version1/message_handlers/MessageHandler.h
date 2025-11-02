#pragma once

#include <smart_home/usp_protocol/include/model/ProtocolMessageHandler.h>
#include <iostream>
#include <vector>

#include "../ReferencedCommonData.h"


namespace smart_home::usp_server::version1::message_handlers {

    class MessageHandler {
    public:
        virtual ~MessageHandler() = default;

        virtual void handleMessage(
            const std::vector<std::shared_ptr<ReferencedCommonData>>& packets
        ) = 0;
    protected:
        template <
            typename TMessage,
            typename TSerialized = std::vector<char>,
            typename THandler = usp_protocol::model::ProtocolMessageHandler<TSerialized, TMessage>,
            typename TSerializationResult = usp_protocol::model::DeserializationResult<TMessage, TSerialized>
        >
        std::vector<std::shared_ptr<TMessage>> buildMessagePackets(
            const std::vector<std::shared_ptr<ReferencedCommonData>>& packets,
            std::unique_ptr<THandler> handler
        ) const {
            std::vector<std::shared_ptr<TMessage>> messages;

            for (const auto& packet : packets) {
                char* rawData = packet->getRawDataReference();
                std::vector<char> vectorBuffer{
                    rawData,
                    rawData + packet->getClient().bytesReceived
                };
                const std::unique_ptr<TSerializationResult> deserializationResult =
                    handler->deserialize(&vectorBuffer);

                if (deserializationResult->isSuccess()) {
                    const std::shared_ptr<TMessage> message =
                        deserializationResult->getDeserializationState();
                    messages.push_back(message);
                } else {
                    std::cerr << "Error deserializing Acknowledgement message" << std::endl;
                }
            }

            return messages;
        }
    };

    class FinalMessage {
    protected:
        std::shared_ptr<ReferencedCommonData> firstPacket;
    public:
        explicit FinalMessage(const std::vector<std::shared_ptr<ReferencedCommonData>>& packets);

        const usp_protocol::ProtocolVersion& protocolVersion;
        const uint16_t& sessionId;
        const uint64_t& timestamp;
        const uint16_t& requestId;
    };

    class FinalDataMessage {
    private:
        static std::vector<char> unifyPacketData(
            const std::vector<char*>& packets,
            size_t sizeIndex
        );
        static std::vector<char> unifyPacketData(
            const std::vector<char*>& packets,
            size_t sizeIndex,
            size_t dataStartIndex
        );
    public:
        size_t size;
        std::string data;

        FinalDataMessage(
            const std::vector<std::shared_ptr<ReferencedCommonData>>& packets,
            const size_t& sizeByte,
            const size_t& dataStartByte
        );
    };
} // namespace smart_home::usp_server::version1::message_handlers