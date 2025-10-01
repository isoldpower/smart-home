#pragma once


namespace smart_home::web_server {

    struct NetServerConfig {
    public:
        const char* ip;
        int port;

        explicit NetServerConfig(
            const char* ip = "127.0.0.1",
            int port = 8080
        );
    };
} // namespace smart_home::web_server