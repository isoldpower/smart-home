#include "../../../include/version1/request/RequestMessageHandler.h"

#include <iostream>
#include <smart_home/utilities/include/BinaryReader.h>


namespace smart_home::usp_protocol::version1 {

    std::unique_ptr<RequestSerializationResult>
        RequestMessageHandler::serialize(RequestMessage* message)
    {
        std::vector<char> resultBuffer{};
        constexpr size_t dataStartIndex = getRequestSegmentIndex(
            RequestSegmentsIndex::DATA_START
        );
        constexpr size_t nullTerminatorSize = 1;
        const size_t finalSize = dataStartIndex + message->data.size() + nullTerminatorSize;
        resultBuffer.reserve(finalSize);

        appendBasis(&resultBuffer, *message);
        appendAction(&resultBuffer, message->action);
        appendAuth(&resultBuffer, message->auth);
        appendData(&resultBuffer, message->data);
        appendGroup(&resultBuffer, message->actionGroup);
        appendPacketIndex(&resultBuffer, message->packetIndex);
        appendPacketsCount(&resultBuffer, message->packetsCount);
        appendSize(&resultBuffer, static_cast<uint8_t>(message->data.size()));

        return std::make_unique<RequestSerializationResult>(
            RequestSerializationResult{
                true,
                message,
                resultBuffer
        });
    }

    std::unique_ptr<RequestDeserializationResult>
        RequestMessageHandler::deserialize(std::vector<char>* buffer)
    {
        const CommonMessageData baseData = basisHandler->parseCommonData(
            buffer->data(),
            buffer->size()
        );
        const size_t bufferSize = buffer->size();
        const char* bufferData = buffer->data();
        const size_t dataSize = determineSize(bufferData, bufferSize);

        RequestMessage resultMessage {
            baseData.protocolVersion,
            baseData.sessionId,
            baseData.timestamp,
            baseData.requestId,
            determinePacketsCount(bufferData, bufferSize),
            determinePacketIndex(bufferData, bufferSize),
            determineAuth(bufferData, bufferSize),
            determineGroup(bufferData, bufferSize),
            determineAction(bufferData, bufferSize),
            dataSize,
            std::string(
                parseDataPointer(bufferData, bufferSize),
                dataSize
            )
        };
        model::DeserializationResult result { true, buffer, resultMessage };

        return std::make_unique<RequestDeserializationResult>(result);
    }

    std::string RequestMessageHandler::determineAuth(const char* buffer, size_t length) const {
        constexpr auto authStartIndex = static_cast<size_t>(RequestSegmentsIndex::AUTH_START);
        constexpr auto authEndIndex = static_cast<size_t>(RequestSegmentsIndex::AUTH_END);

        if (length <= authEndIndex) {
            throw std::out_of_range("Buffer too small to determine auth");
        } else if constexpr (authEndIndex - authStartIndex + 1 != sizeof(uint32_t)) {
            throw std::out_of_range("Auth segment size mismatch");
        } else {
            return { &buffer[authStartIndex], sizeof(uint32_t) };
            // return utilities::BigEndianReader::bytesToUint32(&buffer[authStartIndex]);
        }
    }

    uint8_t RequestMessageHandler::determineGroup(const char* buffer, size_t length) const {
        constexpr auto groupIndex = static_cast<size_t>(RequestSegmentsIndex::GROUP_BYTE);

        if (length <= groupIndex) {
            throw std::out_of_range("Buffer too small to determine group");
        } else {
            return utilities::BigEndianReader::bytesToUint8(&buffer[groupIndex]);
        }
    }

    uint8_t RequestMessageHandler::determineAction(const char* buffer, size_t length) const {
        constexpr auto actionIndex = static_cast<size_t>(RequestSegmentsIndex::ACTION_BYTE);

        if (length <= actionIndex) {
            throw std::out_of_range("Buffer too small to determine action");
        } else {
            return utilities::BigEndianReader::bytesToUint8(&buffer[actionIndex]);
        }
    }

    size_t RequestMessageHandler::determinePacketsCount(const char* buffer, size_t length) const {
        constexpr auto packetsCountIndex = static_cast<size_t>(
            RequestSegmentsIndex::PACKETS_COUNT_BYTE
        );

        if (length <= packetsCountIndex) {
            throw std::out_of_range("Buffer too small to determine packets count");
        } else {
            return utilities::BigEndianReader::bytesToUint8(&buffer[packetsCountIndex]);
        }
    }

    size_t RequestMessageHandler::determinePacketIndex(const char* buffer, size_t length) const {
        constexpr auto packetIndexIndex = static_cast<size_t>(
            RequestSegmentsIndex::PACKET_INDEX_BYTE
        );

        if (length <= packetIndexIndex) {
            throw std::out_of_range("Buffer too small to determine packet index");
        } else {
            return utilities::BigEndianReader::bytesToUint8(&buffer[packetIndexIndex]);
        }
    }


    size_t RequestMessageHandler::determineSize(const char* buffer, size_t length) const {
        constexpr auto sizeIndex = static_cast<size_t>(RequestSegmentsIndex::SIZE_BYTE);

        if (length <= sizeIndex) {
            throw std::out_of_range("Buffer too small to determine size");
        } else {
            return utilities::BigEndianReader::bytesToUint8(&buffer[sizeIndex]);
        }
    }

    char* RequestMessageHandler::parseDataPointer(const char* buffer, size_t length) const {
        constexpr auto dataIndex = static_cast<size_t>(RequestSegmentsIndex::DATA_START);

        if (length <= dataIndex) {
            throw std::out_of_range("Buffer too small to determine data pointer");
        } else {
            return const_cast<char*>(buffer + dataIndex);
        }
    }

    void RequestMessageHandler::appendAuth(
        std::vector<char>* buffer,
        const std::string& auth
    ) const {
        constexpr size_t authStartByte = getRequestSegmentIndex(
            RequestSegmentsIndex::AUTH_START
        );
        constexpr size_t authEndByte = getRequestSegmentIndex(
            RequestSegmentsIndex::AUTH_END
        );
        constexpr size_t bytesCount = authEndByte - authStartByte + 1;

        appendMultiByteField(buffer, authStartByte, bytesCount, auth.data());
    }

    void RequestMessageHandler::appendGroup(
        std::vector<char>* buffer,
        const uint8_t group
    ) const {
        constexpr size_t groupByte = getRequestSegmentIndex(
            RequestSegmentsIndex::GROUP_BYTE
        );
        const std::unique_ptr<char[]> valueBytes = utilities::BigEndianReader::uint8ToBytes(group);

        appendMultiByteField(buffer, groupByte, 1, valueBytes.get());
    }

    void RequestMessageHandler::appendAction(
        std::vector<char>* buffer,
        const uint8_t action
    ) const {
        constexpr size_t actionByte = getRequestSegmentIndex(
            RequestSegmentsIndex::ACTION_BYTE
        );
        const std::unique_ptr<char[]> valueBytes = utilities::BigEndianReader::uint8ToBytes(action);

        appendMultiByteField(buffer, actionByte, 1, valueBytes.get());
    }

    void RequestMessageHandler::appendPacketIndex(
        std::vector<char>* buffer,
        const size_t packetIndex
    ) const {
        constexpr size_t packetIndexByte = getRequestSegmentIndex(
            RequestSegmentsIndex::PACKET_INDEX_BYTE
        );
        const std::unique_ptr<char[]> valueBytes = utilities::BigEndianReader::uint8ToBytes(
            static_cast<uint8_t>(packetIndex)
        );

        appendMultiByteField(buffer, packetIndexByte, 1, valueBytes.get());
    }

    void RequestMessageHandler::appendPacketsCount(
        std::vector<char>* buffer,
        const size_t packetsCount
    ) const {
        constexpr size_t packetsCountByte = getRequestSegmentIndex(
            RequestSegmentsIndex::PACKETS_COUNT_BYTE
        );
        const std::unique_ptr<char[]> valueBytes = utilities::BigEndianReader::uint8ToBytes(
            static_cast<uint8_t>(packetsCount)
        );

        appendMultiByteField(buffer, packetsCountByte, 1, valueBytes.get());
    }

    void RequestMessageHandler::appendSize(
        std::vector<char>* buffer,
        const uint8_t size
    ) const {
        constexpr size_t sizeByte = getRequestSegmentIndex(
            RequestSegmentsIndex::SIZE_BYTE
        );
        const std::unique_ptr<char[]> valueByte = utilities::BigEndianReader::uint8ToBytes(size);

        appendMultiByteField(buffer, sizeByte, 1, valueByte.get());
    }

    void RequestMessageHandler::appendData(
        std::vector<char>* buffer,
        const std::string& data
    ) const {
        constexpr size_t dataStartByte = getRequestSegmentIndex(
            RequestSegmentsIndex::DATA_START
        );
        constexpr size_t nullTerminatorSize = 1;
        const size_t dataSize = data.size() + nullTerminatorSize;

        appendMultiByteField(buffer, dataStartByte, dataSize, data.data());
    }
} // namespace smart_home::usp_protocol::version1