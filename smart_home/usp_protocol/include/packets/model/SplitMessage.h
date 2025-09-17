#pragma once

#include <concepts>


namespace smart_home::usp_protocol::packets::model {

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



    // template <typename T>
    // concept CorrectIndexType =
    //     std::same_as<std::remove_cvref_t<T>, size_t> ||
    //     std::same_as<std::remove_cvref_t<T>, uint8_t>;
    //
    // template <typename T>
    // concept CorrectCountType =
    //     std::same_as<std::remove_cvref_t<T>, size_t> ||
    //     std::same_as<std::remove_cvref_t<T>, uint8_t>;
    //
    // template <typename T>
    // concept HasPacketIndexation = requires {
    //     typename std::remove_cvref_t<decltype(std::declval<T>().packetsCount)>;
    //     typename std::remove_cvref_t<decltype(std::declval<T>().packetIndex)>;
    // }
    // && CorrectCountType<std::remove_cvref_t<decltype(std::declval<T>().packetsCount)>>
    // && CorrectIndexType<std::remove_cvref_t<decltype(std::declval<T>().packetIndex)>>;
} // namespace smart_home::usp_protocol::packets::model