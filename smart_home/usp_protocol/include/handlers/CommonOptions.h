#pragma once

#include <cstdint>


namespace smart_home::usp_protocol::handlers {

    enum class ProtocolVersion : uint8_t {
        VERSION_1 = 1,
    };

    enum class MessageType : uint8_t {
        MESSAGE_REQUEST = 0x00,
        MESSAGE_RESPONSE = 0x01,
        MESSAGE_ACKNOWLEDGEMENT = 0x02,
        MESSAGE_PROTOCOL = 0x03,
        MESSAGE_UNKNOWN = 0xFF,
    };

    enum MessageSettings : unsigned int {
        MAX_PACKET_SIZE = 4096,
        MIN_PACKET_SIZE = 10,
    };
} // namespace smart_home::usp_protocol::handlers