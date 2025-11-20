#pragma once

#include <smart_home/usp_protocol/include/version1/acknowledgement/AcknowledgementMessage.h>
#include <vector>

#include "./MessageHandler.h"


namespace smart_home::usp_server::version1::message_handlers {

    struct FinalAcknowledgementMessage
        : public FinalMessage
        , public FinalDataMessage
    {
    private:
        void assignStatus(
            const std::vector<std::shared_ptr<
                usp_protocol::version1::AcknowledgementMessage
            >>& messages
        );
    public:
        usp_protocol::version1::AcknowledgementStatus status;

        explicit FinalAcknowledgementMessage(
            const std::vector<std::shared_ptr<ReferencedCommonData>>& packets,
            const std::vector<std::shared_ptr<
                usp_protocol::version1::AcknowledgementMessage
            >>& messages
        );
    };

    class AcknowledgementHandler : public MessageHandler {
    public:
        ~AcknowledgementHandler() override = default;
        explicit AcknowledgementHandler(
            std::shared_ptr<utilities::patterns::EventChannel> serverEventChannel
        );

        void handleMessage(
            const std::vector<std::shared_ptr<ReferencedCommonData>>& packets
        ) override;
    };
} // namespace smart_home::usp_server::version1::message_handlers