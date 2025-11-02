#pragma once

#include <smart_home/usp_protocol/include/version1/CommonMessageData.h>
#include <memory>

#include "./MessageHandler.h"


namespace smart_home::usp_server::version1::message_handlers {

    class MessageHandlerBuilder {
    public:
        std::unique_ptr<MessageHandler> buildMessageHandler(
            const usp_protocol::version1::CommonMessagePacketData& commonData
        );
    };
} // namespace smart_home::usp_server::version1::message_handlers