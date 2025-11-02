#include "../../../include/version1/message_handlers/ResponseHandler.h"

#include <iostream>
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

    void ResponseHandler::handleMessage(
        const std::vector<std::shared_ptr<ReferencedCommonData>>& packets
    ) {
        usp_protocol::version1::ResponseMessageHandler handler;
        const std::vector<std::shared_ptr<
            usp_protocol::version1::ResponseMessage
        >> resolvedPackets = buildMessagePackets<usp_protocol::version1::ResponseMessage>(
            packets,
            std::make_unique<usp_protocol::version1::ResponseMessageHandler>()
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
