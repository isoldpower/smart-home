#pragma once

#include <iostream>


namespace smart_home::usp_server::packets {

    template <typename TPacket>
    class PacketPoller {
    public:
        virtual ~PacketPoller() = default;

        virtual int addPacket(const std::string& requestId, TPacket* packet) = 0;
        virtual std::vector<TPacket*> getAllPackets(const std::string& requestId) = 0;
        virtual ssize_t getPacketsCount(const std::string& requestId) = 0;
    };
} // namespace smart_home::usp_server::packets