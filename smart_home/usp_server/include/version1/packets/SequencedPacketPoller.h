#pragma once

#include <smart_home/usp_protocol/include/version1/request/RequestMessage.h>
#include <smart_home/usp_protocol/include/version1/response/ResponseMessage.h>
#include <iostream>
#include <map>
#include <string>

#include "./PacketPoller.h"
#include "./SplitMessage.h"


namespace smart_home::usp_server::version1::packets {

    template <typename TPacket>
    class SequencedPacketPoller : public PacketPoller<TPacket> {
    static_assert(
        HasPacketIndexation<TPacket>,
        "SequencedPacketPoller TPacket template parameter must have packetsCount and packetIndex members "
        "with correct types."
    );
    private:
        std::map<std::string, std::vector<TPacket*>> messagePackets;

        bool isSequenceValid(const std::string& requestId);
        bool isSequenceComplete(const std::string& requestId);
    public:
        int addPacket(const std::string& requestId, TPacket* packet) override;
        std::vector<TPacket*> getAllPackets(const std::string& requestId) override;
        ssize_t getPacketsCount(const std::string& requestId) override;
    };

    extern template class SequencedPacketPoller<usp_protocol::version1::RequestMessage>;
    extern template class SequencedPacketPoller<usp_protocol::version1::ResponseMessage>;
} // namespace smart_home::usp_server::version1::packets