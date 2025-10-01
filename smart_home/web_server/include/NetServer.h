#pragma once

#include "./NetServerClientInfo.h"
#include "./NetServerConfig.h"

#include <sys/select.h>


namespace smart_home::web_server {

    class NetServer {
    private:
        int serverPort;
        int socketDescriptor{-1};

        int associateSocketDescriptor();
        void bindSocketToPort() const;
        [[nodiscard]] bool setListenTimeout(const timeval&) const;
    public:
        explicit NetServer(const NetServerConfig&);

        void establishConnection();
        void closeConnection();
        NetServerClientInfo receiveMessage(size_t, char*, const timeval&) const;
        bool sendMessage(const char*, NetServerClientInfo) const;
    };
} // namespace smart_home::usp_protocol::net