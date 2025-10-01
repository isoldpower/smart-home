#pragma once

#include <cstdint>


namespace smart_home::usp_protocol {

    enum class CommonMessageIndexes : size_t {
        VERSION_BYTE = 0,
        TYPE_BYTE = 1,
        SESSION_ID_START = 2,
        SESSION_ID_END = 3,
        REQUEST_ID_START = 4,
        REQUEST_ID_END = 5,
        TIMESTAMP_START = 6,
        TIMESTAMP_END = 13,
    };

    inline size_t getCommonMessageIndex(const CommonMessageIndexes& index) {
        return static_cast<size_t>(index);
    }

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

    enum MessageSettings : size_t {
        MAX_PACKET_SIZE = 4096,
        COMMON_PACKET_SIZE = static_cast<size_t>(CommonMessageIndexes::TIMESTAMP_END),
        MIN_PACKET_SIZE = static_cast<size_t>(CommonMessageIndexes::TIMESTAMP_END)
    };

    inline uint8_t getProtocolVersion(ProtocolVersion version) {
        return static_cast<uint8_t>(version);
    }
} // namespace smart_home::usp_protocol