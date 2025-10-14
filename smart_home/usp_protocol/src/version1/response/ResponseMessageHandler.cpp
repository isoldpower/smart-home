#include "../../../include/version1/response/ResponseMessageHandler.h"

#include <smart_home/utilities/include/BinaryReader.h>

#include "../../../include/exceptions/ProtocolPacketException.h"


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
        appendSize(&resultBuffer, static_cast<uint8_t>(message->data.size()));

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
        const CommonMessagePacketData baseData = basisHandler->parseCommonData(
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
            baseData.packetIndex,
            baseData.packetsCount,
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

    uint8_t ResponseMessageHandler::determineStatus(const char* buffer, const size_t length) const {
        constexpr auto statusIndex = static_cast<uint8_t>(
            ResponseSegmentsIndex::STATUS_BYTE
        );

        if (length <= statusIndex) {
            throw exceptions::ProtocolPacketException(
                utilities::exceptions::ExceptionLevel::ERROR,
                exceptions::castedExecutionCode(
                    exceptions::ExecutionCodes::RECEIVED_PACKAGE_SIZE_ERROR
                ),
                "Buffer length is too small to determine Status segment value."
            );
        } else {
            return utilities::BigEndianReader::bytesToUint8(&buffer[statusIndex]);
        }
    }

    size_t ResponseMessageHandler::determineSize(const char* buffer, const size_t length) const {
        constexpr auto sizeIndex = static_cast<size_t>(ResponseSegmentsIndex::SIZE_BYTE);

        if (length <= sizeIndex) {
            throw exceptions::ProtocolPacketException(
                utilities::exceptions::ExceptionLevel::ERROR,
                exceptions::castedExecutionCode(
                    exceptions::ExecutionCodes::RECEIVED_PACKAGE_SIZE_ERROR
                ),
                "Buffer length is too small to determine Size segment value."
            );
        } else {
            return utilities::BigEndianReader::bytesToUint8(&buffer[sizeIndex]);
        }
    }

    char* ResponseMessageHandler::parseDataPointer(const char* buffer, const size_t length) const {
        constexpr auto dataIndex = static_cast<size_t>(ResponseSegmentsIndex::DATA_START);

        if (length <= dataIndex) {

            throw exceptions::ProtocolPacketException(
                utilities::exceptions::ExceptionLevel::ERROR,
                exceptions::castedExecutionCode(
                    exceptions::ExecutionCodes::RECEIVED_PACKAGE_SIZE_ERROR
                ),
                "Buffer length is too small to determine Data segment value."
            );
        } else {
            return const_cast<char*>(buffer + dataIndex);
        }
    }

    void ResponseMessageHandler::appendStatus(
        std::vector<char>* buffer,
        const ResponseStatus& status
    ) const {
        constexpr size_t statusByte = getResponseSegmentIndex(
            ResponseSegmentsIndex::STATUS_BYTE
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