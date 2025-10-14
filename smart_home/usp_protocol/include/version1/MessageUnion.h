#pragma once

#include "./acknowledgement/AcknowledgementMessageHandler.h"
#include "./protocol/ProtocolMessageHandler.h"
#include "./request/RequestMessageHandler.h"
#include "./response/ResponseMessageHandler.h"

#include <variant>


namespace smart_home::usp_protocol::version1 {

    struct MessageUnion {
    private:
        std::variant<
            std::unique_ptr<RequestDeserializationResult>,
            std::unique_ptr<ResponseDeserializationResult>,
            std::unique_ptr<AcknowledgementDeserializationResult>,
            std::unique_ptr<ProtocolDeserializationResult>
        > referencePointer;
    public:
        MessageType type;
        bool constructedSuccessfully;
        union {
            RequestMessage* requestMessage;
            ResponseMessage* responseMessage;
            AcknowledgementMessage* acknowledgementMessage;
            ProtocolMessage* protocolMessage;
        } messageVariants;

        MessageUnion(
            const CommonMessageData& commonData,
            const char* buffer,
            size_t length
        );

        ~MessageUnion() = default;
    };

} // namespace smart_home::usp_protocol::version1