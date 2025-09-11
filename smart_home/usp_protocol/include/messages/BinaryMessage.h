#pragma once

#include <array>


namespace smart_home::usp_protocol::messages {

    enum MessageSettings : unsigned int {
        MAX_PACKET_SIZE = 4096
    };

    template <size_t ChunksAmount>
    using BinaryMessageRaw = std::array<const char *, ChunksAmount>;

} // namespace smart_home::usp_protocol::messages