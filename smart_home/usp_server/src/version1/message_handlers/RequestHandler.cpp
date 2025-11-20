#include "../../../include/version1/message_handlers/RequestHandler.h"

#include <iostream>
#include <utility>
#include <smart_home/usp_protocol/include/version1/request/RequestMessage.h>
#include <smart_home/usp_protocol/include/version1/request/RequestMessageHandler.h>


namespace smart_home::usp_server::version1::message_handlers {

    FinalRequestMessage::FinalRequestMessage(
        const std::vector<std::shared_ptr<ReferencedCommonData>> &packets,
        const std::vector<std::shared_ptr<usp_protocol::version1::RequestMessage>> &messages
    )
        : FinalMessage(packets)
        , FinalDataMessage(
            packets,
            usp_protocol::version1::getRequestSegmentIndex(
                usp_protocol::version1::RequestSegmentsIndex::SIZE_BYTE
            ),
            usp_protocol::version1::getRequestSegmentIndex(
                usp_protocol::version1::RequestSegmentsIndex::DATA_START
            )
        )
    {}

    RequestHandler::RequestHandler(
        std::shared_ptr<utilities::patterns::EventChannel> serverEventChannel
    )
        : MessageHandler(std::move(serverEventChannel))
    {}

    void RequestHandler::handleMessage(
        const std::vector<std::shared_ptr<ReferencedCommonData>>& packets
    ) {
        const auto handler = std::make_shared<usp_protocol::version1::RequestMessageHandler>();
        const std::vector<std::shared_ptr<
            usp_protocol::version1::RequestMessage
        >> resolvedPackets = buildMessagePackets<usp_protocol::version1::RequestMessage>(
            packets,
            handler
        );
        const FinalRequestMessage finalMessage(
            packets,
            resolvedPackets
        );

        std::cout << "Final Request Message: " << finalMessage.data << std::endl;
        std::cout << "\tSize: " << finalMessage.size << std::endl;
        std::cout << "\tData Size: " << finalMessage.data.size() << std::endl;
    }
} // namespace smart_home::usp_server::version1::message_handlers
