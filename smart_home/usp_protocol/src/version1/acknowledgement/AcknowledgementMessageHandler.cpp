#include "../../../include/version1/acknowledgement/AcknowledgementMessageHandler.h"

#include <smart_home/utilities/include/BinaryReader.h>
#include <iostream>


namespace smart_home::usp_protocol::version1 {

    std::unique_ptr<AcknowledgementSerializationResult>
        AcknowledgementMessageHandler::serialize(AcknowledgementMessage* message)
    {
        std::vector<char> resultBuffer{};
        const size_t dataStartIndex = getAcknowledgementSegmentIndex(
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

    void AcknowledgementMessageHandler::appendBasis(
        std::vector<char>* buffer,
        const CommonMessageData& data
    ) const {
        size_t basisSize = 0;
        const auto serializedBasis = basisHandler->serializeCommonData(
            data,
            basisSize
        );

        if (buffer->capacity() <= basisSize) {
            throw std::out_of_range("Buffer too small to append basis data");
        } else if (buffer->size() <= basisSize) {
            buffer->resize(basisSize);
        }

        std::copy_n(
            serializedBasis.get(),
            basisSize,
            buffer->begin()
        );
    }

    void AcknowledgementMessageHandler::appendStatus(
        std::vector<char>* buffer,
        const AcknowledgementStatus& status
    ) const {
        const size_t statusByte = getAcknowledgementSegmentIndex(
            AcknowledgementSegmentsIndex::STATUS_BYTE
        );

        if (buffer->capacity() <= statusByte) {
            throw std::out_of_range("Buffer too small to append status");
        } else if (buffer->size() <= statusByte) {
            buffer->resize(statusByte + 1);
        }

        const auto primitiveValue = static_cast<uint8_t>(status);
        (*buffer)[statusByte] = utilities::BigEndianReader::uint8ToBytes(primitiveValue)[0];
    }

    void AcknowledgementMessageHandler::appendSize(
        std::vector<char>* buffer,
        uint8_t size
    ) const {
        const size_t sizeByte = getAcknowledgementSegmentIndex(
            AcknowledgementSegmentsIndex::SIZE_BYTE
        );

        if (buffer->capacity() <= sizeByte) {
            throw std::out_of_range("Buffer too small to append size");
        } else if (buffer->size() <= sizeByte) {
            buffer->resize(sizeByte + 1);
        }

        (*buffer)[sizeByte] = utilities::BigEndianReader::uint8ToBytes(size)[0];
    }

    void AcknowledgementMessageHandler::appendData(
        std::vector<char>* buffer,
        const std::string& data
    ) const {
        const size_t dataStartByte = getAcknowledgementSegmentIndex(
            AcknowledgementSegmentsIndex::DATA_START
        );
        constexpr size_t nullTerminatorSize = 1;
        const size_t requiredSize = dataStartByte + data.size() + nullTerminatorSize;

        if (buffer->capacity() < requiredSize) {
            throw std::out_of_range("Buffer too small to append data. Expected size: "
                + std::to_string(requiredSize)
                + ", capacity: "
                + std::to_string(buffer->capacity())
            );
        } else if (buffer->size() < requiredSize) {
            buffer->resize(requiredSize);
        }

        std::copy_n(
            data.data(),
            data.size(),
            buffer->begin() + static_cast<long>(dataStartByte)
        );
    }

} // namespace smart_home::usp_protocol::version1