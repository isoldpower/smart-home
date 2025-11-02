#pragma once

#include <smart_home/utilities/include/concepts/TypeConcepts.h>
#include <smart_home/utilities/include/concepts/PointerTraits.h>
#include <iostream>
#include <map>

#include "./PacketPoller.h"
#include "../ReferencedCommonData.h"


namespace smart_home::usp_server::version1::packets {

    template<typename T>
    concept HasPacketIndexation =
        requires {
            typename utilities::concepts::member_pointer_traits<decltype(&T::packetsCount)>::member_type;
            typename utilities::concepts::member_pointer_traits<decltype(&T::packetIndex)>::member_type;
        } &&
        utilities::concepts::CountOrIndexType<
            typename utilities::concepts::member_pointer_traits<
                decltype(&T::packetsCount)
            >::member_type> &&
        utilities::concepts::CountOrIndexType<
            typename utilities::concepts::member_pointer_traits<
                decltype(&T::packetIndex)
            >::member_type>;


    template <typename TRequestId, typename TPacket>
    class SequencedPacketPoller : public PacketPoller<TRequestId, TPacket> {
    static_assert(
        HasPacketIndexation<TPacket>,
        "SequencedPacketPoller TPacket template parameter must have packetsCount and packetIndex members "
        "with correct types."
    );
        using PacketEntry = std::pair<TRequestId, std::shared_ptr<TPacket>>;
    private:
        std::multimap<TRequestId, std::shared_ptr<TPacket>> messagePackets;

        bool isSequenceValid(const TRequestId& requestId);
    public:
        int addPacket(const TRequestId& requestId, std::shared_ptr<TPacket> packet) override;
        std::vector<std::shared_ptr<TPacket>> getAllPackets(const TRequestId& requestId) override;
        ssize_t getPacketsCount(const TRequestId& requestId) override;
        bool isSequenceComplete(const TRequestId& requestId) override;
    };

    extern template class SequencedPacketPoller<
        uint16_t,
        ReferencedCommonData
    >;
} // namespace smart_home::usp_server::version1::packets