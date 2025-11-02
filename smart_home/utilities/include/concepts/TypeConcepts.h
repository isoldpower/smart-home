#pragma once

#include <iterator>
#include <concepts>


namespace smart_home::utilities::concepts {

    template<typename M>
    concept CountOrIndexType =
        std::same_as<std::remove_cv_t<M>, size_t> ||
        std::same_as<std::remove_cv_t<M>, uint8_t>;

    template<typename T>
    concept Iterable = requires(T container) {
        { std::begin(container) } -> std::input_or_output_iterator;
        { std::end(container) } -> std::input_or_output_iterator;
        requires std::same_as<
            decltype(std::begin(container)),
            decltype(std::end(container))
        >;
    };
} // namespace smart_home::utilities::concepts