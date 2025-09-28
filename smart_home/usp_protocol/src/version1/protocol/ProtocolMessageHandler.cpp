#include "../../../include/version1/protocol/ProtocolMessageHandler.h"

#include <smart_home/utilities/include/BinaryReader.h>


namespace smart_home::usp_protocol::version1 {

    std::unique_ptr<ProtocolSerializationResult>
        ProtocolMessageHandler::serialize(ProtocolMessage* message)
    {
        std::vector<char> resultBuffer{};
        constexpr size_t dataStartIndex = getProtocolSegmentIndex(
            ProtocolSegmentsIndex::DATA_START
        );
        constexpr size_t nullTerminatorSize = 1;
        const size_t finalSize = dataStartIndex + message->data.size() + nullTerminatorSize;
        resultBuffer.reserve(finalSize);

        appendBasis(&resultBuffer, *message);
        appendAction(&resultBuffer, message->action);
        appendSize(&resultBuffer, message->size);
        appendData(&resultBuffer, message->data);

        return std::make_unique<ProtocolSerializationResult>(
            ProtocolSerializationResult{
                true,
                message,
                resultBuffer
        });
    }

    std::unique_ptr<ProtocolDeserializationResult>
        ProtocolMessageHandler::deserialize(std::vector<char>* buffer)
    {
        const CommonMessageData baseData = basisHandler->parseCommonData(
            buffer->data(),
            buffer->size()
        );
        const size_t bufferSize = buffer->size();
        const char* bufferData = buffer->data();
        const size_t dataSize = determineSize(bufferData, bufferSize);

        const ProtocolMessage resultMessage {
            baseData.protocolVersion,
            baseData.sessionId,
            baseData.timestamp,
            baseData.requestId,
            static_cast<ProtocolAction>(
                determineAction(bufferData, bufferSize)
            ),
            dataSize,
            std::string(
                parseDataPointer(bufferData, bufferSize),
                dataSize
            )
        };
        model::DeserializationResult result { true, buffer, resultMessage };

        return std::make_unique<ProtocolDeserializationResult>(result);
    }

    uint8_t ProtocolMessageHandler::determineAction(const char* buffer, size_t length) const {
        constexpr auto actionIndex = static_cast<size_t>(ProtocolSegmentsIndex::ACTION_BYTE);

        if (length <= actionIndex) {
            throw std::out_of_range("Buffer too small to determine action");
        } else {
            return utilities::BigEndianReader::bytesToUint8(&buffer[actionIndex]);
        }
    }

    size_t ProtocolMessageHandler::determineSize(const char* buffer, size_t length) const {
        constexpr auto sizeIndex = static_cast<size_t>(ProtocolSegmentsIndex::SIZE_BYTE);

        if (length <= sizeIndex) {
            throw std::out_of_range("Buffer too small to determine size");
        } else {
            return utilities::BigEndianReader::bytesToUint8(&buffer[sizeIndex]);
        }
    }

    char* ProtocolMessageHandler::parseDataPointer(const char* buffer, size_t length) const {
        constexpr auto dataIndex = static_cast<size_t>(ProtocolSegmentsIndex::DATA_START);

        if (length <= dataIndex) {
            throw std::out_of_range("Buffer too small to determine data pointer");
        } else {
            return const_cast<char*>(buffer + dataIndex);
        }
    }

    void ProtocolMessageHandler::appendAction(
        std::vector<char>* buffer,
        ProtocolAction action
    ) const {
        constexpr auto actionIndex = static_cast<size_t>(
            ProtocolSegmentsIndex::ACTION_BYTE
        );
        const std::unique_ptr<char[]> actionValue = utilities::BigEndianReader::uint8ToBytes(
            static_cast<size_t>(action)
        );

        appendMultiByteField(buffer, actionIndex, 1, actionValue.get());
    }

    void ProtocolMessageHandler::appendSize(
        std::vector<char>* buffer,
        const uint8_t size
    ) const {
        constexpr size_t sizeByte = getProtocolSegmentIndex(
            ProtocolSegmentsIndex::SIZE_BYTE
        );
        const std::unique_ptr<char[]> valueByte = utilities::BigEndianReader::uint8ToBytes(size);
        appendMultiByteField(buffer, sizeByte, 1, valueByte.get());
    }

    void ProtocolMessageHandler::appendData(
        std::vector<char>* buffer,
        const std::string& data
    ) const {
        constexpr size_t dataStartByte = getProtocolSegmentIndex(
            ProtocolSegmentsIndex::DATA_START
        );
        constexpr size_t nullTerminatorSize = 1;
        const size_t dataSize = data.size() + nullTerminatorSize;

        appendMultiByteField(buffer, dataStartByte, dataSize, data.data());
    }
} // namespace smart_home::usp_protocol::version1