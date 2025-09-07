#pragma once


namespace smart_home::usp_protocol::server {

    struct UspServerConfig {
        const char* ip;
        int port;
    };

    inline UspServerConfig DEFAULT_CONFIG = {
        "127.0.0.1",
        8080
    };
}