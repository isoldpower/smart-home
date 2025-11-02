#pragma once

#include <smart_home/usp_protocol/include/version1/protocol/ProtocolMessage.h>
#include <vector>

#include "./MessageHandler.h"


namespace smart_home::usp_server::version1::message_handlers {

    struct FinalProtocolMessage
        : public FinalMessage
        , public FinalDataMessage
    {
    public:
        explicit FinalProtocolMessage(
            const std::vector<std::shared_ptr<ReferencedCommonData>>& packets,
            const std::vector<std::shared_ptr<
                usp_protocol::version1::ProtocolMessage
            >>& messages
        );
    };

    class ProtocolHandler : public MessageHandler {
    public:
        ~ProtocolHandler() override = default;

        void handleMessage(
            const std::vector<std::shared_ptr<ReferencedCommonData>>& packets
        ) override;
    };
} // namespace smart_home::usp_server::version1::message_handlers