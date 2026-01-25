#pragma once


namespace smart_home::utilities::concepts {

    template <typename>
    struct member_pointer_traits;

    template <typename M, typename C>
    struct member_pointer_traits<M C::*> {
        using member_type = M;
        using class_type  = C;
    };
} // namespace smart_home::utilities::concepts