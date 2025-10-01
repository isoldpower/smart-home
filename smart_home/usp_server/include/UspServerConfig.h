#pragma once

#include <smart_home/usp_protocol/include/version1/request/RequestMessage.h>

#include "./packets/PacketPoller.h"


namespace smart_home::usp_server {

    using namespace smart_home::usp_protocol;

    struct UspServerConfig {
    public:
        const char* host;
        const int port;
        packets::PacketPoller<version1::RequestMessage>* requestPacketPoller;

        UspServerConfig(const char* host, int port);
    };
} // namespace smart_home::usp_server