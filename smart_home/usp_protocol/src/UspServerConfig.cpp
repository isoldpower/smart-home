#include "../include/UspServerConfig.h"


namespace smart_home::usp_protocol {

    UspServerConfig::UspServerConfig(const char* host, int port)
        : host(host)
        , port(port)
    {}
} // namespace smart_home::usp_protocol