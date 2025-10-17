#include "../../../include/version1/message_handlers/MessageHandlerBuilder.h"

#include "../../../include/version1/message_handlers/AcknowledgementHandler.h"
#include "../../../include/version1/message_handlers/ProtocolHandler.h"
#include "../../../include/version1/message_handlers/RequestHandler.h"
#include "../../../include/version1/message_handlers/ResponseHandler.h"
#include "../../../include/exceptions/ReceivedMessageException.h"


namespace smart_home::usp_server::version1::message_handlers {

    std::unique_ptr<MessageHandler> MessageHandlerBuilder::buildMessageHandler(
        const usp_protocol::version1::CommonMessagePacketData &commonData
    ) {
        switch (commonData.type) {
            case usp_protocol::version1::MessageType::MESSAGE_ACKNOWLEDGEMENT:
                return std::make_unique<AcknowledgementHandler>();
            case usp_protocol::version1::MessageType::MESSAGE_REQUEST:
                return std::make_unique<RequestHandler>();
            case usp_protocol::version1::MessageType::MESSAGE_RESPONSE:
                return std::make_unique<ResponseHandler>();
            case usp_protocol::version1::MessageType::MESSAGE_PROTOCOL:
                return std::make_unique<ProtocolHandler>();
            case usp_protocol::version1::MessageType::MESSAGE_UNKNOWN:
                throw exceptions::ReceivedMessageException(
                    exceptions::ExceptionLevel::WARNING,
                    exceptions::ExecutionCodes::UNKNOWN_MESSAGE_TYPE,
                    "Received message with unknown type; skipping processing."
                );
            default:
                throw exceptions::ReceivedMessageException(
                    exceptions::ExceptionLevel::FATAL,
                    exceptions::ExecutionCodes::UNKNOWN_MESSAGE_TYPE,
                    "Received message type that is not defined in internal server handling process"
                );
        }
    }

} // namespace smart_home::usp_server::version1::message_handlers
