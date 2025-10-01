#pragma once

#include <concepts>
#include <cstdint>


namespace smart_home::usp_server::packets {

    namespace detail {
        template<typename> struct member_pointer_traits;

        template<typename M, typename C>
        struct member_pointer_traits<M C::*> {
            using member_type = M;
            using class_type  = C;
        };
    }

    template<typename M>
    concept CountOrIndexType =
        std::same_as<std::remove_cv_t<M>, size_t> ||
        std::same_as<std::remove_cv_t<M>, uint8_t>;

    template<typename T>
    concept HasPacketIndexation =
        requires {
            typename detail::member_pointer_traits<decltype(&T::packetsCount)>::member_type;
            typename detail::member_pointer_traits<decltype(&T::packetIndex)>::member_type;
        } &&
        CountOrIndexType<typename detail::member_pointer_traits<decltype(&T::packetsCount)>::member_type> &&
        CountOrIndexType<typename detail::member_pointer_traits<decltype(&T::packetIndex)>::member_type>;
} // namespace smart_home::usp_server::packets