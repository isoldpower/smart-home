#pragma once

#include "./acknowledgement/AcknowledgementMessageHandler.h"
#include "./protocol/ProtocolMessageHandler.h"
#include "./request/RequestMessageHandler.h"
#include "./response/ResponseMessageHandler.h"

#include <variant>


namespace smart_home::usp_protocol {

    struct MessageUnion {
    private:
        std::variant<
            std::unique_ptr<version1::RequestDeserializationResult>,
            std::unique_ptr<version1::ResponseDeserializationResult>,
            std::unique_ptr<version1::AcknowledgementDeserializationResult>,
            std::unique_ptr<version1::ProtocolDeserializationResult>
        > referencePointer;
    public:
        MessageType type;
        bool constructedSuccessfully;
        union {
            version1::RequestMessage* requestMessage;
            version1::ResponseMessage* responseMessage;
            version1::AcknowledgementMessage* acknowledgementMessage;
            version1::ProtocolMessage* protocolMessage;
        } messageVariants;

        MessageUnion(
            const CommonMessageData& commonData,
            const char* buffer,
            size_t length
        );

        ~MessageUnion() = default;
    };

} // namespace smart_home::usp_protocol