#pragma once

#include <array>


namespace smart_home::usp_protocol::handlers::version1 {

    template <size_t ChunksAmount>
    using BinaryMessageRaw = std::array<const char *, ChunksAmount>;

} // namespace smart_home::usp_protocol::handlers::version1