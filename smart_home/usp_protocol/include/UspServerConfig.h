#pragma once


namespace smart_home::usp_protocol {

    struct UspServerConfig {
        const char* host;
        const int port;
    };
} // namespace smart_home::usp_protocol