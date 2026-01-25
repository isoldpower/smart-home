#pragma once

#include <memory>
#include <vector>


namespace smart_home::usp_server::version1::packets {

    template <typename TPacket>
    class PacketUnifier {
    public:
        virtual ~PacketUnifier() = default;

        virtual std::shared_ptr<TPacket> unifyPackets(
            const std::vector<std::shared_ptr<TPacket>>& packets
        ) = 0;
    };
} // namespace smart_home::usp_server::version1::packets