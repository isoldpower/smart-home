#pragma once

#include <cstdint>


namespace smart_home::usp_protocol {

    enum class ProtocolVersion : uint8_t {
        VERSION_1 = 0x01,
    };

    enum class MessageType : uint8_t {
        MESSAGE_REQUEST = 0x01,
        MESSAGE_RESPONSE = 0x02,
        MESSAGE_ACKNOWLEDGEMENT = 0x03,
        MESSAGE_PROTOCOL = 0x04,
        MESSAGE_UNKNOWN = 0xFF,
    };

    enum MessageSettings : unsigned int {
        MAX_PACKET_SIZE = 4096,
        MIN_PACKET_SIZE = 10,
    };

    inline uint8_t getProtocolVersion(ProtocolVersion version) {
        return static_cast<uint8_t>(version);
    }
} // namespace smart_home::usp_protocol