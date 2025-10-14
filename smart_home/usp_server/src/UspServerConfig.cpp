#include "../include/UspServerConfig.h"


namespace smart_home::usp_server {

    UspServerConfig::UspServerConfig(
        const char* host,
        const int port,
        const timeval& receiveIterationHang
    )
        : host(host)
        , port(port)
        , receiveIterationHang(receiveIterationHang)
    {}
} // namespace smart_home::usp_server