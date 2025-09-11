#pragma once

#include <sys/select.h>

#include "./NetServerClientInfo.h"
#include "./NetServerConfig.h"

namespace smart_home::usp_protocol::net {

    class NetServer {
    private:
        int serverPort;
        int socketDescriptor{-1};

        int associateSocketDescriptor();
        void bindSocketToPort() const;
        bool setListenTimeout(const timeval&) const;
    public:
        explicit NetServer(const NetServerConfig& = DEFAULT_CONFIG);

        void establishConnection();
        void closeConnection();
        NetServerClientInfo receiveMessage(size_t, char*, const timeval&) const;
        bool sendMessage(const char*, NetServerClientInfo) const;
    };
} // namespace smart_home::usp_protocol::net