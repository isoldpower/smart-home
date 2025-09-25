#include "../../../include/version1/acknowledgement/AcknowledgementMessageHandler.h"

#include <iostream>
#include <smart_home/utilities/include/BinaryReader.h>


namespace smart_home::usp_protocol::version1 {

    std::unique_ptr<AcknowledgementSerializationResult>
        AcknowledgementMessageHandler::serialize(AcknowledgementMessage* message)
    {
        // TODO: Implement actual serialization logic
        char resultBuffer[] = "Acknowledgement Example";
        AcknowledgementSerializationResult result {
            true,
            message,
            std::vector(
                resultBuffer,
                resultBuffer + sizeof(resultBuffer) / sizeof(char)
            )
        };

        return std::make_unique<AcknowledgementSerializationResult>(result);
    }

    std::unique_ptr<AcknowledgementDeserializationResult>
        AcknowledgementMessageHandler::deserialize(std::vector<char>* buffer)
    {
        const CommonMessageData baseData = basisHandler->parseCommonData(
            buffer->data(),
            buffer->size()
        );
        const size_t bufferSize = buffer->size();
        const char* bufferData = buffer->data();
        const size_t dataSize = determineSize(bufferData, bufferSize);

        const AcknowledgementMessage resultMessage {
            baseData.protocolVersion,
            baseData.sessionId,
            baseData.timestamp,
            baseData.requestId,
            static_cast<AcknowledgementStatus>(
                determineStatus(bufferData, bufferSize)
            ),
            dataSize,
            std::string(
                parseDataPointer(bufferData, bufferSize),
                 dataSize
            )
        };
        model::DeserializationResult result { true, buffer, resultMessage };

        return std::make_unique<AcknowledgementDeserializationResult>(result);
    }

    uint8_t AcknowledgementMessageHandler::determineStatus(
        const char* buffer,
        const size_t length
    ) const {
        constexpr auto statusIndex = static_cast<size_t>(
            AcknowledgementSegmentsIndex::STATUS_BYTE
        );

        if (length <= statusIndex) {
            throw std::out_of_range("Buffer too small to determine status");
        } else {
            return utilities::BigEndianReader::bytesToUint8(&buffer[statusIndex]);
        }
    }

    uint8_t AcknowledgementMessageHandler::determineSize(
        const char* buffer,
        const size_t length
    ) const {
        constexpr auto sizeIndex = static_cast<size_t>(
            AcknowledgementSegmentsIndex::SIZE_BYTE
        );

        if (length <= sizeIndex) {
            throw std::out_of_range("Buffer too small to determine size");
        } else {
            return utilities::BigEndianReader::bytesToUint8(&buffer[sizeIndex]);
        }
    }

    char* AcknowledgementMessageHandler::parseDataPointer(
        const char* buffer,
        const size_t length
    ) const {
        constexpr auto dataIndex = static_cast<size_t>(
            AcknowledgementSegmentsIndex::DATA_START
        );

        if (length <= dataIndex) {
            throw std::out_of_range("Buffer too small to parse data pointer");
        } else {
            return const_cast<char*>(buffer + dataIndex);
        }
    }
} // namespace smart_home::usp_protocol::version1