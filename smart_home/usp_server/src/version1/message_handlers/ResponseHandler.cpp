#include "../../../include/version1/message_handlers/ResponseHandler.h"

#include <iostream>
#include <utility>
#include <smart_home/usp_protocol/include/version1/response/ResponseMessage.h>
#include <smart_home/usp_protocol/include/version1/response/ResponseMessageHandler.h>


namespace smart_home::usp_server::version1::message_handlers {

    FinalResponseMessage::FinalResponseMessage(
        const std::vector<std::shared_ptr<ReferencedCommonData>> &packets,
        const std::vector<std::shared_ptr<usp_protocol::version1::ResponseMessage>> &messages
    )
        : FinalMessage(packets)
        , FinalDataMessage(
            packets,
            usp_protocol::version1::getResponseSegmentIndex(
                usp_protocol::version1::ResponseSegmentsIndex::SIZE_BYTE
            ),
            usp_protocol::version1::getResponseSegmentIndex(
                usp_protocol::version1::ResponseSegmentsIndex::DATA_START
            )
        )
    {}

    ResponseHandler::ResponseHandler(
        std::shared_ptr<utilities::patterns::EventChannel> serverEventChannel
    )
        : MessageHandler(std::move(serverEventChannel))
    {}

    void ResponseHandler::handleMessage(
        const std::vector<std::shared_ptr<ReferencedCommonData>>& packets
    ) {
        const auto handler = std::make_shared<usp_protocol::version1::ResponseMessageHandler>();
        const std::vector<std::shared_ptr<
            usp_protocol::version1::ResponseMessage
        >> resolvedPackets = buildMessagePackets<usp_protocol::version1::ResponseMessage>(
            packets,
            handler
        );
        const FinalResponseMessage finalMessage(
            packets,
            resolvedPackets
        );

        std::cout << "Final Response Message: " << finalMessage.data << std::endl;
        std::cout << "\tSize: " << finalMessage.size << std::endl;
        std::cout << "\tData Size: " << finalMessage.data.size() << std::endl;
    }
} // namespace smart_home::usp_server::version1::message_handlers
