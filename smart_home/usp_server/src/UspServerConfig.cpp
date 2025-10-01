#include "../include/UspServerConfig.h"

#include "../include/packets/SequencedPacketPoller.h"


namespace smart_home::usp_server {

    UspServerConfig::UspServerConfig(
        const char* host,
        int port
    )
        : host(host)
        , port(port)
        , requestPacketPoller(
            new packets::SequencedPacketPoller<version1::RequestMessage>()
        )
    {}
} // namespace smart_home::usp_server