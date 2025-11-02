#include "../../../include/version1/message_handlers/RequestHandler.h"

#include <iostream>
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

    void RequestHandler::handleMessage(
        const std::vector<std::shared_ptr<ReferencedCommonData>>& packets
    ) {
        usp_protocol::version1::RequestMessageHandler handler;
        const std::vector<std::shared_ptr<
            usp_protocol::version1::RequestMessage
        >> resolvedPackets = buildMessagePackets<usp_protocol::version1::RequestMessage>(
            packets,
            std::make_unique<usp_protocol::version1::RequestMessageHandler>()
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
