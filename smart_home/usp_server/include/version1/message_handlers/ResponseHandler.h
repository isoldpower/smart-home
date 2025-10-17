#pragma once

#include "./MessageHandler.h"


namespace smart_home::usp_server::version1::message_handlers {

    class ResponseHandler : public
    MessageHandler {
    public:
        ~ResponseHandler() override = default;

        void handleMessage(const char* buffer, UspServerClient client) override;
    };
} // namespace smart_home::usp_server::version1::message_handlers