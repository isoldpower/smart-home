#pragma once

#include <smart_home/utilities/include/concepts/TypeConcepts.h>
#include <vector>


namespace smart_home::usp_server::version1::packets {

    template <typename TId, typename TPacket>
    class PacketPoller {
    public:
        virtual ~PacketPoller() = default;

        virtual int addPacket(const TId& requestId, std::shared_ptr<TPacket> packet) = 0;
        virtual std::vector<std::shared_ptr<TPacket>> getAllPackets(const TId& requestId) = 0;
        virtual ssize_t getPacketsCount(const TId& requestId) = 0;
        virtual bool isSequenceComplete(const TId& requestId) = 0;
        virtual void freeSequence(const TId& requestId) = 0;
    };
} // namespace smart_home::usp_server::version1::packets