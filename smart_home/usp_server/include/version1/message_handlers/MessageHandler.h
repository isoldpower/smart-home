#pragma once

#include "../../UspServerClient.h"


namespace smart_home::usp_server::version1::message_handlers {

    class MessageHandler {
    public:
        virtual ~MessageHandler() = default;

        virtual void handleMessage(const char* buffer, UspServerClient client) = 0;
    };
} // namespace smart_home::usp_server::version1::message_handlers