#include "../../../include/version1/packets/SequencedPacketPoller.h"


namespace smart_home::usp_server::version1::packets {

    template <typename TPacket>
    ssize_t SequencedPacketPoller<TPacket>::getPacketsCount(const std::string& requestId) {
        if (isSequenceComplete(requestId) == false) {
            return -1;
        }

        return messagePackets[requestId].size();
    }

    template <typename TPacket>
    std::vector<TPacket*> SequencedPacketPoller<TPacket>::getAllPackets(const std::string& requestId) {
        if (isSequenceComplete(requestId) == false) {
            return std::vector<TPacket*>{};
        }

        std::vector<TPacket*> packets = messagePackets.at(requestId);
        std::sort(
            packets.begin(),
            packets.end(),
            [](TPacket* a, TPacket* b) {
                return a->packetIndex < b->packetIndex;
            }
        );

        return packets;
    }

    template <typename TPacket>
    int SequencedPacketPoller<TPacket>::addPacket(const std::string& requestId, TPacket* packet) {
        try {
            if (messagePackets.contains(requestId) == false) {
                messagePackets.insert({ requestId, std::vector<TPacket*>() });
            }

            messagePackets.at(requestId).push_back(packet);
            return packet->packetsCount == messagePackets.at(requestId).size();
        } catch (const std::exception& e) {
            std::cerr << "Error adding packet to poller: " << e.what() << std::endl;
            return -1;
        }
    }

    template <typename TPacket>
    bool SequencedPacketPoller<TPacket>::isSequenceComplete(const std::string& requestId) {
        if (messagePackets.contains(requestId) == false) {
            return false;
        }

        const std::vector<TPacket*>& packets = messagePackets[requestId];
        std::map<size_t, size_t> indexCountMap;
        for (const auto& iterationPacket : packets) {
            indexCountMap[iterationPacket->packetIndex] += 1;
        }

        const bool sequenceValid = isSequenceValid(requestId);
        const bool allIndexesPresent = indexCountMap.size() == packets[0]->packetsCount;
        const bool noDuplicateIndexes = std::all_of(
            indexCountMap.begin(),
            indexCountMap.end(),
            [](const auto& iterationPacket) {
                return iterationPacket.second == 1;
            }
        );

        return sequenceValid && allIndexesPresent && noDuplicateIndexes;
    }

    template <typename TPacket>
    bool SequencedPacketPoller<TPacket>::isSequenceValid(const std::string& requestId) {
        if (messagePackets.contains(requestId) == false) {
            return false;
        }

        const std::vector<TPacket*>& packets = messagePackets[requestId];
        size_t expectedCount = packets[0]->packetsCount;
        const bool validPacketsCount = packets.size() == expectedCount;
        const bool indexesInRange = std::all_of(
            packets.begin(),
            packets.end(),
            [expectedCount](TPacket* packet) {
                return packet->packetIndex < expectedCount && packet->packetIndex >= 0;
            }
        );

        return validPacketsCount && indexesInRange;
    }

    template class SequencedPacketPoller<usp_protocol::version1::RequestMessage>;
    template class SequencedPacketPoller<usp_protocol::version1::ResponseMessage>;
} // namespace smart_home::usp_server::version1::packets
