#include "../../../include/version1/packets/SequencedPacketPoller.h"


namespace smart_home::usp_server::version1::packets {

    template <typename TRequestId, typename TPacket>
    ssize_t SequencedPacketPoller<TRequestId, TPacket>::getPacketsCount(
        const TRequestId& requestId
    ) {
        if (isSequenceComplete(requestId) == false) {
            return -1;
        }

        return messagePackets.count(requestId);
    }

    template <typename TRequestId, typename TPacket>
    std::vector<std::shared_ptr<TPacket>> SequencedPacketPoller<TRequestId, TPacket>::getAllPackets(
        const TRequestId& requestId
    ) {
        if (isSequenceComplete(requestId) == false) {
            return std::vector<std::shared_ptr<TPacket>>{};
        }

        auto packetsRange = messagePackets.equal_range(requestId);
        std::vector<std::shared_ptr<TPacket>> packetsVector;
        std::transform(
            packetsRange.first,
            packetsRange.second,
            std::back_inserter(packetsVector),
            [](const PacketEntry& pair) {
                return pair.second;
            }
        );

        return packetsVector;
    }

    template <typename TRequestId, typename TPacket>
    int SequencedPacketPoller<TRequestId, TPacket>::addPacket(
        const TRequestId& requestId,
        std::shared_ptr<TPacket> packet
    ) {
        try {
            messagePackets.emplace(requestId, packet);
            return packet->packetsCount == messagePackets.count(requestId);
        } catch (const std::exception& e) {
            std::cerr << "Error adding packet to poller: " << e.what() << std::endl;
            return -1;
        }
    }

    template <typename TRequestId, typename TPacket>
    bool SequencedPacketPoller<TRequestId, TPacket>::isSequenceComplete(
        const TRequestId& requestId
    ) {
        if (messagePackets.contains(requestId) == false) {
            return false;
        }

        const auto packetsRange = messagePackets.equal_range(requestId);
        const size_t packetsCount = std::distance(packetsRange.first, packetsRange.second);
        std::map<size_t, size_t> indexCountMap;
        std::for_each(
            packetsRange.first,
            packetsRange.second,
            [&indexCountMap](const PacketEntry& pair) {
                indexCountMap[pair.second->packetIndex] += 1;
            }
        );

        const bool sequenceValid = isSequenceValid(requestId);
        const bool allIndexesPresent = indexCountMap.size() == packetsCount;
        const bool noDuplicateIndexes = std::all_of(
            indexCountMap.begin(),
            indexCountMap.end(),
            [](const auto& iterationPacket) {
                return iterationPacket.second == 1;
            }
        );

        return sequenceValid && allIndexesPresent && noDuplicateIndexes;
    }

    template <typename TRequestId, typename TPacket>
    bool SequencedPacketPoller<TRequestId, TPacket>::isSequenceValid(
        const TRequestId& requestId
    ) {
        if (messagePackets.contains(requestId) == false) {
            return false;
        }

        auto packetsRange = messagePackets.equal_range(requestId);
        const size_t detectedPacketsCount = std::distance(
            packetsRange.first,
            packetsRange.second
        );
        if (detectedPacketsCount == 0) return false;

        const std::shared_ptr<TPacket>& firstPacket = packetsRange.first->second;
        const size_t expectedPacketsCount = firstPacket->packetsCount;

        const bool validPacketsCount = detectedPacketsCount == expectedPacketsCount;
        const bool indexesInRange = std::all_of(
            packetsRange.first,
            packetsRange.second,
            [expectedPacketsCount](const PacketEntry& pair) {
                const std::shared_ptr<TPacket>& packet = pair.second;
                const bool isValidCount = packet->packetsCount == expectedPacketsCount;
                const bool isValidIndex =
                    packet->packetIndex < expectedPacketsCount &&
                    packet->packetIndex >= 0;

                return isValidCount && isValidIndex;
            }
        );

        return validPacketsCount && indexesInRange;
    }

    template class SequencedPacketPoller<uint16_t, ReferencedCommonData>;
} // namespace smart_home::usp_server::version1::packets
