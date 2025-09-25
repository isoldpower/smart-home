#include "../../../include/version1/request/RequestMessageHandler.h"

#include <iostream>
#include <smart_home/utilities/include/BinaryReader.h>


namespace smart_home::usp_protocol::version1 {

    std::unique_ptr<RequestSerializationResult>
        RequestMessageHandler::serialize(RequestMessage* message)
    {
        // TODO: Implement proper serialization logic
        char resultBuffer[] = "Request Example";
        RequestSerializationResult result {
            true,
            message,
            std::vector(
                resultBuffer,
                resultBuffer + sizeof(resultBuffer) / sizeof(char)
            )
        };

        return std::make_unique<RequestSerializationResult>(result);
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
} // namespace smart_home::usp_protocol::version1