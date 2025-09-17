#pragma once

#include "./packets/model/PacketPoller.h"
#include "./handlers/version1/request/RequestMessage.h"

namespace smart_home::usp_protocol {

    struct UspServerConfig {
    public:
        const char* host;
        const int port;
        packets::model::PacketPoller<handlers::version1::RequestMessage>* requestPacketPoller;

        UspServerConfig(const char* host, int port);
    };
} // namespace smart_home::usp_protocol