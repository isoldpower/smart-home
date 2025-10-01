#include "../../../include/version1/acknowledgement/AcknowledgementMessageHandler.h"

#include <smart_home/utilities/include/BinaryReader.h>
#include <iostream>

#include "../../../include/exceptions/ProtocolPacketException.h"


namespace smart_home::usp_protocol::version1 {

    std::unique_ptr<AcknowledgementSerializationResult>
        AcknowledgementMessageHandler::serialize(AcknowledgementMessage* message)
    {
        std::vector<char> resultBuffer{};
        constexpr size_t dataStartIndex = getAcknowledgementSegmentIndex(
            AcknowledgementSegmentsIndex::DATA_START
        );
        constexpr size_t nullTerminatorSize = 1;
        const size_t finalSize = dataStartIndex + message->data.size() + nullTerminatorSize;
        resultBuffer.reserve(finalSize);

        appendBasis(&resultBuffer, *message);
        appendStatus(&resultBuffer, message->status);
        appendData(&resultBuffer, message->data);
        appendSize(&resultBuffer, static_cast<uint8_t>(message->data.size()));

        return std::make_unique<AcknowledgementSerializationResult>(
            AcknowledgementSerializationResult{
                true,
                message,
                resultBuffer
        });
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
            throw exceptions::ProtocolPacketException(
                utilities::exceptions::ExceptionLevel::ERROR,
                castedExecutionCode(ExecutionCodes::RECEIVED_PACKAGE_SIZE_ERROR),
                "Buffer length is too small to determine ACK status."
            );
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
            throw exceptions::ProtocolPacketException(
                utilities::exceptions::ExceptionLevel::ERROR,
                castedExecutionCode(ExecutionCodes::RECEIVED_PACKAGE_SIZE_ERROR),
                "Buffer length is too small to determine Data Size."
            );
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
            throw exceptions::ProtocolPacketException(
                utilities::exceptions::ExceptionLevel::ERROR,
                castedExecutionCode(ExecutionCodes::RECEIVED_PACKAGE_SIZE_ERROR),
                "Buffer length is too small to determine Data pointer."
            );
        } else {
            return const_cast<char*>(buffer + dataIndex);
        }
    }

    void AcknowledgementMessageHandler::appendStatus(
        std::vector<char>* buffer,
        const AcknowledgementStatus& status
    ) const {
        constexpr size_t statusByte = getAcknowledgementSegmentIndex(
            AcknowledgementSegmentsIndex::STATUS_BYTE
        );
        const std::unique_ptr<char[]> valueBytes = utilities::BigEndianReader::uint8ToBytes(
            static_cast<uint8_t>(status)
        );
        appendMultiByteField(buffer, statusByte, 1, valueBytes.get());
    }

    void AcknowledgementMessageHandler::appendSize(
        std::vector<char>* buffer,
        const uint8_t size
    ) const {
        constexpr size_t sizeByte = getAcknowledgementSegmentIndex(
            AcknowledgementSegmentsIndex::SIZE_BYTE
        );
        const std::unique_ptr<char[]> valueByte = utilities::BigEndianReader::uint8ToBytes(size);
        appendMultiByteField(buffer, sizeByte, 1, valueByte.get());
    }

    void AcknowledgementMessageHandler::appendData(
        std::vector<char>* buffer,
        const std::string& data
    ) const {
        constexpr size_t dataStartByte = getAcknowledgementSegmentIndex(
            AcknowledgementSegmentsIndex::DATA_START
        );
        constexpr size_t nullTerminatorSize = 1;
        const size_t dataSize = data.size() + nullTerminatorSize;

        appendMultiByteField(buffer, dataStartByte, dataSize, data.data());
    }

} // namespace smart_home::usp_protocol::version1