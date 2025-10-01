#pragma once

#include "./version1/acknowledgement/AcknowledgementMessageHandler.h"
#include "./version1/protocol/ProtocolMessageHandler.h"
#include "./version1/request/RequestMessageHandler.h"
#include "./version1/response/ResponseMessageHandler.h"

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