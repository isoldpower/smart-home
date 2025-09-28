#include "../../../include/version1/response/ResponseMessageHandler.h"

#include <smart_home/utilities/include/BinaryReader.h>


namespace smart_home::usp_protocol::version1 {

    std::unique_ptr<ResponseSerializationResult>
        ResponseMessageHandler::serialize(ResponseMessage* message)
    {
        std::vector<char> resultBuffer{};
        constexpr size_t dataStartIndex = getResponseSegmentIndex(
            ResponseSegmentsIndex::DATA_START
        );
        constexpr size_t nullTerminatorSize = 1;
        const size_t finalSize = dataStartIndex + message->data.size() + nullTerminatorSize;
        resultBuffer.reserve(finalSize);

        appendBasis(&resultBuffer, *message);
        appendData(&resultBuffer, message->data);
        appendStatus(&resultBuffer, message->status);
        appendPacketsCount(&resultBuffer, message->packetsCount);
        appendPacketIndex(&resultBuffer, message->packetIndex);
        appendSize(&resultBuffer, message->data.size());

        return std::make_unique<ResponseSerializationResult>(
            ResponseSerializationResult{
                true,
                message,
                resultBuffer
        });
    }

    std::unique_ptr<ResponseDeserializationResult>
        ResponseMessageHandler::deserialize(std::vector<char>* buffer)
    {
        CommonMessageData baseData = basisHandler->parseCommonData(
            buffer->data(),
            buffer->size()
        );
        const size_t bufferSize = buffer->size();
        const char* bufferData = buffer->data();
        const size_t dataSize = determineSize(bufferData, bufferSize);

        const ResponseMessage resultMessage {
            baseData.protocolVersion,
            baseData.sessionId,
            baseData.timestamp,
            baseData.requestId,
            determinePacketsCount(bufferData, bufferSize),
            determinePacketIndex(bufferData, bufferSize),
            static_cast<ResponseStatus>(
                determineStatus(bufferData, bufferSize)
            ),
            dataSize,
            std::string(
                parseDataPointer(bufferData, bufferSize),
                dataSize
            )
        };
        model::DeserializationResult result { true, buffer, resultMessage };

        return std::make_unique<ResponseDeserializationResult>(result);
    }

    size_t ResponseMessageHandler::determinePacketsCount(const char* buffer, size_t length) const {
        constexpr auto packetsCountIndex = static_cast<size_t>(
            ResponseSegmentsIndex::PACKETS_COUNT_BYTE
        );

        if (length <= packetsCountIndex) {
            throw std::out_of_range("Buffer too small to determine packets count");
        } else {
            return utilities::BigEndianReader::bytesToUint8(&buffer[packetsCountIndex]);
        }
    }

    size_t ResponseMessageHandler::determinePacketIndex(const char* buffer, size_t length) const {
        constexpr auto packetIndexIndex = static_cast<size_t>(
            ResponseSegmentsIndex::PACKET_INDEX_BYTE
        );

        if (length <= packetIndexIndex) {
            throw std::out_of_range("Buffer too small to determine packet index");
        } else {
            return utilities::BigEndianReader::bytesToUint8(&buffer[packetIndexIndex]);
        }
    }

    uint8_t ResponseMessageHandler::determineStatus(const char* buffer, size_t length) const {
        constexpr auto statusIndex = static_cast<uint8_t>(
            ResponseSegmentsIndex::STATUS_BYTE
        );

        if (length <= statusIndex) {
            throw std::out_of_range("Buffer too small to determine response status");
        } else {
            return utilities::BigEndianReader::bytesToUint8(&buffer[statusIndex]);
        }
    }

    size_t ResponseMessageHandler::determineSize(const char* buffer, size_t length) const {
        constexpr auto sizeIndex = static_cast<size_t>(ResponseSegmentsIndex::SIZE_BYTE);

        if (length <= sizeIndex) {
            throw std::out_of_range("Buffer too small to determine size");
        } else {
            return utilities::BigEndianReader::bytesToUint8(&buffer[sizeIndex]);
        }
    }

    char* ResponseMessageHandler::parseDataPointer(const char* buffer, size_t length) const {
        constexpr auto dataIndex = static_cast<size_t>(ResponseSegmentsIndex::DATA_START);

        if (length <= dataIndex) {
            throw std::out_of_range("Buffer too small to determine data pointer");
        } else {
            return const_cast<char*>(buffer + dataIndex);
        }
    }

    void ResponseMessageHandler::appendPacketIndex(
        std::vector<char>* buffer,
        const size_t packetIndex
    ) const {
        constexpr size_t packetIndexByte = getResponseSegmentIndex(
            ResponseSegmentsIndex::PACKET_INDEX_BYTE
        );
        const std::unique_ptr<char[]> valueBytes = utilities::BigEndianReader::uint8ToBytes(
            static_cast<uint8_t>(packetIndex)
        );

        appendMultiByteField(buffer, packetIndexByte, 1, valueBytes.get());
    }

    void ResponseMessageHandler::appendPacketsCount(
        std::vector<char>* buffer,
        const size_t packetsCount
    ) const {
        constexpr size_t packetsCountByte = getResponseSegmentIndex(
            ResponseSegmentsIndex::PACKETS_COUNT_BYTE
        );
        const std::unique_ptr<char[]> valueBytes = utilities::BigEndianReader::uint8ToBytes(
            static_cast<uint8_t>(packetsCount)
        );

        appendMultiByteField(buffer, packetsCountByte, 1, valueBytes.get());
    }

    void ResponseMessageHandler::appendStatus(
        std::vector<char>* buffer,
        const ResponseStatus& status
    ) const {
        constexpr size_t statusByte = getResponseSegmentIndex(
            ResponseSegmentsIndex::PACKETS_COUNT_BYTE
        );
        const std::unique_ptr<char[]> valueBytes = utilities::BigEndianReader::uint8ToBytes(
            static_cast<uint8_t>(status)
        );

        appendMultiByteField(buffer, statusByte, 1, valueBytes.get());
    }

    void ResponseMessageHandler::appendSize(
        std::vector<char>* buffer,
        const size_t size
    ) const {
        constexpr size_t sizeByte = getResponseSegmentIndex(
            ResponseSegmentsIndex::SIZE_BYTE
        );
        const std::unique_ptr<char[]> valueByte = utilities::BigEndianReader::uint8ToBytes(
            static_cast<uint8_t>(size)
        );

        appendMultiByteField(buffer, sizeByte, 1, valueByte.get());
    }

    void ResponseMessageHandler::appendData(
        std::vector<char>* buffer,
        const std::string& data
    ) const {
        constexpr size_t dataStartByte = getResponseSegmentIndex(
            ResponseSegmentsIndex::DATA_START
        );
        constexpr size_t nullTerminatorSize = 1;
        const size_t dataSize = data.size() + nullTerminatorSize;

        appendMultiByteField(buffer, dataStartByte, dataSize, data.data());
    }
} // namespace smart_home::usp_protocol::version1