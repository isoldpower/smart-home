#include "../../../include/version1/message_handlers/ProtocolHandler.h"

#include <iostream>
#include <smart_home/usp_protocol/include/version1/protocol/ProtocolMessage.h>
#include <smart_home/usp_protocol/include/version1/protocol/ProtocolMessageHandler.h>


namespace smart_home::usp_server::version1::message_handlers {

    FinalProtocolMessage::FinalProtocolMessage(
        const std::vector<std::shared_ptr<ReferencedCommonData>> &packets,
        const std::vector<std::shared_ptr<usp_protocol::version1::ProtocolMessage>> &messages
    )
        : FinalMessage(packets)
        , FinalDataMessage(
            packets,
            usp_protocol::version1::getProtocolSegmentIndex(
                usp_protocol::version1::ProtocolSegmentsIndex::SIZE_BYTE
            ),
            usp_protocol::version1::getProtocolSegmentIndex(
                usp_protocol::version1::ProtocolSegmentsIndex::DATA_START
            )
        )
    {}

    void ProtocolHandler::handleMessage(
        const std::vector<std::shared_ptr<ReferencedCommonData>>& packets
    ) {
        const std::vector<std::shared_ptr<
            usp_protocol::version1::ProtocolMessage
        >> resolvedPackets = buildMessagePackets<usp_protocol::version1::ProtocolMessage>(
            packets,
            std::make_unique<usp_protocol::version1::ProtocolMessageHandler>()
        );
        const FinalProtocolMessage finalMessage(
            packets,
            resolvedPackets
        );

        std::cout << "Final Protocol Message: " << finalMessage.data << std::endl;
        std::cout << "\tSize: " << finalMessage.size << std::endl;
        std::cout << "\tData Size: " << finalMessage.data.size() << std::endl;
    }
} // namespace smart_home::usp_server::version1::message_handlers
