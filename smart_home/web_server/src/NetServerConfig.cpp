#include "../include/NetServerConfig.h"


namespace smart_home::web_server {

    NetServerConfig::NetServerConfig(
        const char* ip,
        const int port
    )
        : ip(ip)
        , port(port)
    {}
} // namespace smart_home::web_server