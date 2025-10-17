#pragma once

#include <smart_home/usp_protocol/include/version1/request/RequestMessage.h>

#include "./MessageHandler.h"
#include "../packets/PacketPoller.h"


namespace smart_home::usp_server::version1::message_handlers {

    class RequestHandler : public MessageHandler {
    private:
        packets::PacketPoller<usp_protocol::version1::RequestMessage>* packetPoller;
    public:
        RequestHandler();
        ~RequestHandler() override;

        void handleMessage(const char* buffer, UspServerClient client) override;
    };
    
} // namespace smart_home::usp_server::version1::message_handlers