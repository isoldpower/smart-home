#include "../include/UspServerConfig.h"

#include "../include/packets/SequencedPacketPoller.h"


namespace smart_home::usp_server {

    UspServerConfig::UspServerConfig(
        const char* host,
        const int port,
        const timeval& receiveIterationHang
    )
        : host(host)
        , port(port)
        , receiveIterationHang(receiveIterationHang)
        , requestPacketPoller(
            new packets::SequencedPacketPoller<version1::RequestMessage>()
        )
    {}
} // namespace smart_home::usp_server