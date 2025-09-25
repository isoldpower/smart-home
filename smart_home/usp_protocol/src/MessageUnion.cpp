#include "../include/MessageUnion.h"

#include <iostream>

namespace smart_home::usp_protocol {

    MessageUnion::MessageUnion(
        const CommonMessageData& commonData,
        const char* buffer,
        const size_t length
    )
        : type(commonData.type)
        , messageVariants({})
    {
        auto bufferVector = std::vector(buffer, buffer + length);

        switch (type) {
            case MessageType::MESSAGE_REQUEST: {
                version1::RequestMessageHandler handler{};
                referencePointer = handler.deserialize(&bufferVector);

                messageVariants.requestMessage = std::get<
                    std::unique_ptr<version1::RequestDeserializationResult>
                >(referencePointer)->getDeserializationState();
                constructedSuccessfully = std::get<
                    std::unique_ptr<version1::RequestDeserializationResult>
                >(referencePointer)->isSuccess();

                break;
            }
            case MessageType::MESSAGE_RESPONSE: {
                version1::ResponseMessageHandler handler{};
                referencePointer = handler.deserialize(&bufferVector);

                messageVariants.responseMessage = std::get<
                    std::unique_ptr<version1::ResponseDeserializationResult>
                >(referencePointer)->getDeserializationState();
                constructedSuccessfully = std::get<
                    std::unique_ptr<version1::ResponseDeserializationResult>
                >(referencePointer)->getDeserializationState();

                break;
            }
            case MessageType::MESSAGE_ACKNOWLEDGEMENT: {
                version1::AcknowledgementMessageHandler handler{};
                referencePointer = handler.deserialize(&bufferVector);

                messageVariants.acknowledgementMessage = std::get<
                    std::unique_ptr<version1::AcknowledgementDeserializationResult>
                >(referencePointer)->getDeserializationState();
                constructedSuccessfully = std::get<
                    std::unique_ptr<version1::AcknowledgementDeserializationResult>
                >(referencePointer)->isSuccess();

                break;
            }
            case MessageType::MESSAGE_PROTOCOL: {
                version1::ProtocolMessageHandler handler{};
                referencePointer = handler.deserialize(&bufferVector);

                messageVariants.protocolMessage = std::get<
                    std::unique_ptr<version1::ProtocolDeserializationResult>
                >(referencePointer)->getDeserializationState();
                constructedSuccessfully = std::get<
                    std::unique_ptr<version1::ProtocolDeserializationResult>
                >(referencePointer)->isSuccess();

                break;
            }
            default: {
                throw std::invalid_argument("Unsupported message type");
            }
        }
    }
} // namespace smart_home::usp_protocol