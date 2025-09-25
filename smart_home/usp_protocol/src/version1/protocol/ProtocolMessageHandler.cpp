#include "../../../include/version1/protocol/ProtocolMessageHandler.h"

#include <smart_home/utilities/include/BinaryReader.h>


namespace smart_home::usp_protocol::version1 {

    std::unique_ptr<ProtocolSerializationResult>
        ProtocolMessageHandler::serialize(ProtocolMessage* message)
    {
        // TODO: Implement proper serialization logic
        char resultBuffer[] = "Protocol Example";
        ProtocolSerializationResult result {
            true,
            message,
            std::vector(
                resultBuffer,
                resultBuffer + sizeof(resultBuffer) / sizeof(char)
            )
        };

        return std::make_unique<ProtocolSerializationResult>(result);
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
} // namespace smart_home::usp_protocol::version1