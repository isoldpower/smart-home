#include "../../../include/version1/message_handlers/AcknowledgementHandler.h"

#include <iostream>
#include <utility>
#include <smart_home/usp_protocol/include/version1/acknowledgement/AcknowledgementMessage.h>
#include <smart_home/usp_protocol/include/version1/acknowledgement/AcknowledgementMessageHandler.h>

#include "../../../include/version1/events/MessageReceivedEvent.h"
#include "../../../include/version1/events/RequestSentEvent.h"
#include "../../../include/version1/events/ServerEvents.h"


namespace smart_home::usp_server::version1::message_handlers {

    FinalAcknowledgementMessage::FinalAcknowledgementMessage(
        const std::vector<std::shared_ptr<ReferencedCommonData>>& packets,
        const std::vector<std::shared_ptr<usp_protocol::version1::AcknowledgementMessage>>& messages
    )
        : FinalMessage(packets)
        , FinalDataMessage(
            packets,
            usp_protocol::version1::getAcknowledgementSegmentIndex(
                usp_protocol::version1::AcknowledgementSegmentsIndex::SIZE_BYTE
            ),
            usp_protocol::version1::getAcknowledgementSegmentIndex(
                usp_protocol::version1::AcknowledgementSegmentsIndex::DATA_START
            )
        )
        , status(usp_protocol::version1::AcknowledgementStatus::ACKNOWLEDGEMENT_UNKNOWN)
    {
        assignStatus(messages);
    }

    void FinalAcknowledgementMessage::assignStatus(
        const std::vector<std::shared_ptr<
            usp_protocol::version1::AcknowledgementMessage
        >>& messages
    ) {
        std::vector<usp_protocol::version1::AcknowledgementStatus> errorStatuses;
        const bool isSuccess = std::ranges::all_of(
            messages,
            [&errorStatuses](
                const std::shared_ptr<usp_protocol::version1::AcknowledgementMessage>& message
            ) {
                if (message->status != usp_protocol::version1::AcknowledgementStatus::ACKNOWLEDGEMENT_SUCCESS) {
                    errorStatuses.push_back(message->status);
                    return false;
                }

                return true;
            }
        );

        if (isSuccess && errorStatuses.empty()) {
            this->status = messages.front()->status;
        } else {
            this->status = errorStatuses[0];
        }
    }

    AcknowledgementHandler::AcknowledgementHandler(
        std::shared_ptr<utilities::patterns::EventChannel> serverEventChannel
    )
        : MessageHandler(std::move(serverEventChannel))
    {
        this->serverEventChannel->subscribe<events::MessageReceivedEvent>(
            events::getEventName(events::ServerEvent::MessageReceivedEvent),
            [&](const events::MessageReceivedEvent& event) -> std::any {
                callForMessageType(
                    usp_protocol::version1::MessageType::MESSAGE_ACKNOWLEDGEMENT,
                    event.packetsList,
                    [this, event]() {
                        handleMessage(event.packetsList);
                    }
                );

                return 0;
            }
        );

        this->serverEventChannel->subscribe<events::RequestSentEvent>(
            events::getEventName(events::ServerEvent::RequestSentEvent),
            [&](const events::RequestSentEvent& event) -> std::any {
            }
        );
    }

    void AcknowledgementHandler::handleMessage(
        const std::vector<std::shared_ptr<ReferencedCommonData>>& packets
    ) {
        const auto handler = std::make_shared<usp_protocol::version1::AcknowledgementMessageHandler>();
        const std::vector<std::shared_ptr<
            usp_protocol::version1::AcknowledgementMessage
        >> resolvedPackets = buildMessagePackets<usp_protocol::version1::AcknowledgementMessage>(
            packets,
            handler
        );
        const FinalAcknowledgementMessage finalMessage(
            packets,
            resolvedPackets
        );

        std::cout << "Final Acknowledgement Message: " << finalMessage.data << std::endl;
        std::cout << "\tSize: " << finalMessage.size << std::endl;
        std::cout << "\tData Size: " << finalMessage.data.size() << std::endl;
    }
} // namespace smart_home::usp_server::version1::message_handlers
