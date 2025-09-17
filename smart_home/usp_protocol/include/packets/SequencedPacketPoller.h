#pragma once

#include <map>

#include "./model/PacketPoller.h"
#include "./model/SplitMessage.h"


namespace smart_home::usp_protocol::packets {

    template <typename TPacket>
    class SequencedPacketPoller : public model::PacketPoller<TPacket> {
    static_assert(
        model::HasPacketIndexation<TPacket>,
        "SequencedPacketPoller TPacket template parameter must have packetsCount and packetIndex members "
        "with correct types."
    );
    private:
        std::map<std::string, std::vector<TPacket*>> messagePackets;

        bool isSequenceValid(const std::string& requestId) {
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

        bool isSequenceComplete(const std::string& requestId) {
            if (messagePackets.contains(requestId) == false) {
                return false;
            }

            const std::vector<TPacket*>& packets = messagePackets[requestId];
            std::map<size_t, size_t> indexCountMap;
            for (const auto& iterationPacket : packets) {
                indexCountMap[iterationPacket->packetIndex] += 1;
            }

            bool sequenceValid = isSequenceValid(requestId);
            bool allIndexesPresent = indexCountMap.size() == packets[0]->packetsCount;
            bool noDuplicateIndexes = std::all_of(
                indexCountMap.begin(),
                indexCountMap.end(),
                [](const auto& iterationPacket) {
                    return iterationPacket.second == 1;
                }
            );

            return sequenceValid && allIndexesPresent && noDuplicateIndexes;
        }
    public:
        int addPacket(const std::string& requestId, TPacket* packet) override {
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

        std::vector<TPacket*> getAllPackets(const std::string& requestId) override {
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

        ssize_t getPacketsCount(const std::string& requestId) override {
            if (isSequenceComplete(requestId) == false) {
                return -1;
            }

            return messagePackets[requestId].size();
        }
    };
} // namespace smart_home::usp_protocol::packets