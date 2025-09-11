#pragma once


namespace smart_home::usp_protocol {

    struct UspServerConfig {
    public:
        const char* host;
        const int port;

        UspServerConfig(const char* host, int port);
    };
} // namespace smart_home::usp_protocol