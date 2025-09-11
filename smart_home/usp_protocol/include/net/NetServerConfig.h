#pragma once


namespace smart_home::usp_protocol::net {

    struct NetServerConfig {
        const char* ip;
        int port;
    };

    inline NetServerConfig DEFAULT_CONFIG = {
        "127.0.0.1",
        8080
    };
}