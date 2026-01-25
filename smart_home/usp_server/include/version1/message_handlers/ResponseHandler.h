#pragma once

#include <smart_home/usp_protocol/include/version1/response/ResponseMessage.h>
#include <vector>

#include "./MessageHandler.h"


namespace smart_home::usp_server::version1::message_handlers {

    struct FinalResponseMessage
        : public FinalMessage
        , public FinalDataMessage
    {
    public:
        explicit FinalResponseMessage(
            const std::vector<std::shared_ptr<ReferencedCommonData>>& packets,
            const std::vector<std::shared_ptr<
                usp_protocol::version1::ResponseMessage
            >>& messages
        );
    };

    class ResponseHandler : public MessageHandler {
    public:
        ~ResponseHandler() override = default;
        explicit ResponseHandler(
            std::shared_ptr<utilities::patterns::EventChannel> serverEventChannel
        );

        void handleMessage(
            const std::vector<std::shared_ptr<ReferencedCommonData>>& packets
        ) override;
    };
} // namespace smart_home::usp_server::version1::message_handlers