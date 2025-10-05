#pragma once


namespace smart_home::usp_protocol {

    enum class ProtocolVersion : uint8_t {
        VERSION_1 = 0x01,
    };

    consteval uint8_t getProtocolVersion(ProtocolVersion version) {
        return static_cast<uint8_t>(version);
    }
} // namespace smart_home::usp_protocol