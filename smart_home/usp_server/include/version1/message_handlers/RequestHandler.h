#pragma once

#include <smart_home/usp_protocol/include/version1/request/RequestMessage.h>
#include <vector>

#include "./MessageHandler.h"


namespace smart_home::usp_server::version1::message_handlers {

    struct FinalRequestMessage
        : public FinalMessage
        , public FinalDataMessage
    {
    public:
        explicit FinalRequestMessage(
            const std::vector<std::shared_ptr<ReferencedCommonData>>& packets,
            const std::vector<std::shared_ptr<
                usp_protocol::version1::RequestMessage
            >>& messages
        );
    };

    class RequestHandler : public MessageHandler {
    public:
        ~RequestHandler() override = default;

        void handleMessage(
            const std::vector<std::shared_ptr<ReferencedCommonData>>& packets
        ) override;
    };

} // namespace smart_home::usp_server::version1::message_handlers