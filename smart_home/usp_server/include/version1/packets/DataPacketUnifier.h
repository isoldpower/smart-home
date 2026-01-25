#pragma once

#include <smart_home/utilities/include/concepts/PointerTraits.h>
#include <smart_home/utilities/include/concepts/TypeConcepts.h>

#include "./PacketUnifier.h"


namespace smart_home::usp_server::version1::packets {

    template<typename T>
    concept HasDataChunk =
        requires {
            typename utilities::concepts::member_pointer_traits<decltype(&T::size)>::member_type;
            typename utilities::concepts::member_pointer_traits<decltype(&T::data)>::member_type;
        } &&
        utilities::concepts::CountOrIndexType<
            typename utilities::concepts::member_pointer_traits<
                decltype(&T::size)
        >::member_type> &&
        utilities::concepts::Iterable<
            typename utilities::concepts::member_pointer_traits<
                decltype(&T::data)
        >::member_type>;


    template <typename TPacket>
    class DataPacketUnifier : public PacketUnifier<TPacket> {
        static_assert(
            HasDataChunk<TPacket>,
            "PacketUnifier TPacket template parameter must have a 'data' field that is iterable, "
            "and a 'size' field that indicates the size of the data."
        );
    public:
        std::shared_ptr<TPacket> unifyPackets(
            const std::vector<std::shared_ptr<TPacket>>& packets
        ) override {
            TPacket resultPacket;
        }
    };
} // namespace smart_home::usp_server::version1::packets