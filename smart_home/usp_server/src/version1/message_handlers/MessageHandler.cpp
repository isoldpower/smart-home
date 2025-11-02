#include "../../../include/version1/message_handlers/MessageHandler.h"

#include <iostream>
#include <smart_home/utilities/include/BinaryReader.h>


namespace smart_home::usp_server::version1::message_handlers {

    FinalMessage::FinalMessage(
        const std::vector<std::shared_ptr<ReferencedCommonData>>& packets
    )
        : firstPacket(packets.front())
        , protocolVersion(firstPacket->protocolVersion)
        , sessionId(firstPacket->sessionId)
        , timestamp(std::ranges::max_element(
            packets,
            [](
                const std::shared_ptr<ReferencedCommonData>& a,
                const std::shared_ptr<ReferencedCommonData>& b
            ) {
                return a->timestamp < b->timestamp;
            }
        )->get()->timestamp)
        , requestId(firstPacket->requestId)
    {}

    FinalDataMessage::FinalDataMessage(
        const std::vector<std::shared_ptr<ReferencedCommonData>> &packets,
        const size_t& sizeByte,
        const size_t& dataStartByte
    ) {
        std::vector<char*> packetBuffers;
        std::ranges::transform(
            packets,
            std::back_inserter(packetBuffers),
            [](const std::shared_ptr<ReferencedCommonData>& packet) {
                return packet->getRawDataReference();
            }
        );

        std::vector<char> unifiedData = unifyPacketData(
            packetBuffers,
            sizeByte,
            dataStartByte
        );
        this->size = unifiedData.size();
        this->data = std::string(unifiedData.begin(), unifiedData.end());
    }

    std::vector<char> FinalDataMessage::unifyPacketData(
        const std::vector<char*>& packets,
        const size_t sizeIndex
    ) {
        return unifyPacketData(
            packets,
            sizeIndex,
            sizeIndex + 1
        );
    }

    std::vector<char> FinalDataMessage::unifyPacketData(
        const std::vector<char*>& packets,
        const size_t sizeIndex,
        const size_t dataStartIndex
    ) {
        if (packets.empty()) {
            return {};
        }

        std::vector<char> resultData;
        for (const auto& packet : packets) {
            const uint8_t packetSize = utilities::BigEndianReader::bytesToUint8(&packet[sizeIndex]);

            resultData.insert(
                resultData.end(),
                packet + dataStartIndex,
                packet + dataStartIndex + packetSize
            );
        }

        return resultData;
    }

} // namespace smart_home::usp_server::version1::message_handlers
