#pragma once

#include <iostream>

#include "./UspServerConfig.h"
#include "./UspServerClientInfo.h"


namespace smart_home::usp_protocol::server {

    class UspServer {
    private:
        int serverPort;
        int socketDescriptor{-1};

        int associateSocketDescriptor();
        void bindSocketToPort() const;
    public:
        explicit UspServer(const UspServerConfig& = DEFAULT_CONFIG);

        void establishConnection();
        void closeConnection();
        UspServerClientInfo receiveMessage(size_t, char*) const;
        bool sendMessage(const char*, UspServerClientInfo) const;
    };
} // namespace smart_home::usp_protocol::server