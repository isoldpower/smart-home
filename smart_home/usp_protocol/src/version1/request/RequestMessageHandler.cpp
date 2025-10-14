#include "../../../include/version1/request/RequestMessageHandler.h"

#include <smart_home/utilities/include/BinaryReader.h>
#include <iostream>

#include "../../../include/exceptions/ProtocolStructuralException.h"
#include "../../../include/exceptions/ProtocolSerializationException.h"


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
        const CommonMessagePacketData baseData = basisHandler->parseCommonData(
            buffer->data(),
            buffer->size()
        );
        const size_t bufferSize = buffer->size();
        const char* bufferData = buffer->data();
        const size_t dataSize = determineSize(bufferData, bufferSize);

        const RequestMessage resultMessage {
            baseData.protocolVersion,
            baseData.sessionId,
            baseData.timestamp,
            baseData.requestId,
            baseData.packetIndex,
            baseData.packetsCount,
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
            throw exceptions::ProtocolStructuralException(
                utilities::exceptions::ExceptionLevel::ERROR,
                exceptions::castedExecutionCode(
                    exceptions::ExecutionCodes::RECEIVED_PACKAGE_SIZE_ERROR
                ),
                "Buffer length is too small to determine Auth segment value."
            );
        } else if constexpr (authEndIndex - authStartIndex + 1 != sizeof(uint32_t)) {
            throw exceptions::ProtocolSerializationException(
                utilities::exceptions::ExceptionLevel::ERROR,
                exceptions::castedExecutionCode(
                    exceptions::ExecutionCodes::WRONG_VERSION_STRUCTURE
                ),
                "Auth size mismatch between passed type size and space available in the protocol message structure."
            );
        } else {
            return { &buffer[authStartIndex], sizeof(uint32_t) };
        }
    }

    uint8_t RequestMessageHandler::determineGroup(const char* buffer, size_t length) const {
        constexpr auto groupIndex = static_cast<size_t>(RequestSegmentsIndex::GROUP_BYTE);

        if (length <= groupIndex) {
            throw exceptions::ProtocolStructuralException(
                utilities::exceptions::ExceptionLevel::ERROR,
                exceptions::castedExecutionCode(
                    exceptions::ExecutionCodes::RECEIVED_PACKAGE_SIZE_ERROR
                ),
                "Buffer length is too small to determine Group segment value."
            );
        } else {
            return utilities::BigEndianReader::bytesToUint8(&buffer[groupIndex]);
        }
    }

    uint8_t RequestMessageHandler::determineAction(const char* buffer, size_t length) const {
        constexpr auto actionIndex = static_cast<size_t>(RequestSegmentsIndex::ACTION_BYTE);

        if (length <= actionIndex) {
            throw exceptions::ProtocolStructuralException(
                utilities::exceptions::ExceptionLevel::ERROR,
                exceptions::castedExecutionCode(
                    exceptions::ExecutionCodes::RECEIVED_PACKAGE_SIZE_ERROR
                ),
                "Buffer length is too small to determine Action segment value."
            );
        } else {
            return utilities::BigEndianReader::bytesToUint8(&buffer[actionIndex]);
        }
    }

    size_t RequestMessageHandler::determineSize(const char* buffer, size_t length) const {
        constexpr auto sizeIndex = static_cast<size_t>(RequestSegmentsIndex::SIZE_BYTE);

        if (length <= sizeIndex) {
            throw exceptions::ProtocolStructuralException(
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

    char* RequestMessageHandler::parseDataPointer(const char* buffer, size_t length) const {
        constexpr auto dataIndex = static_cast<size_t>(RequestSegmentsIndex::DATA_START);

        if (length <= dataIndex) {
            throw exceptions::ProtocolStructuralException(
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