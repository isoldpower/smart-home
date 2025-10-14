#pragma once

#include <algorithm>
#include <iostream>
#include <vector>

#include "../exceptions/ExecutionCodes.h"
#include "../exceptions/ProtocolAllocationException.h"
#include "../model/ProtocolMessageHandler.h"
#include "./MessageBasisHandler.h"


namespace smart_home::usp_protocol::version1 {

    template <typename MessageType>
    class BinaryMessageHandler
        : public model::ProtocolMessageHandler<std::vector<char>, MessageType> {
    protected:
        MessageBasisHandler* basisHandler;

        void validateBufferAllocations(
            std::vector<char>* buffer,
            const size_t requiredSize
        ) const {
            if (buffer->capacity() < requiredSize) {
                const std::string warningMessage = "Buffer is too small to append passed data,"
                                                   " extra reallocation(s) required. Expected size: "
                    + std::to_string(requiredSize)
                    + ", capacity: "
                    + std::to_string(buffer->capacity());
                const auto exception = exceptions::ProtocolAllocationException(
                    utilities::exceptions::ExceptionLevel::WARNING,
                    exceptions::castedExecutionCode(
                        exceptions::ExecutionCodes::INEFFICIENT_MEMORY_ALLOCATION
                    ),
                    warningMessage.c_str()
                );

                std::cout << exception.what() << std::endl;
                buffer->reserve(requiredSize);
            }

            if (buffer->size() <= requiredSize) {
                buffer->resize(requiredSize);
            }
        }

        void appendBasis(std::vector<char>* buffer, const CommonMessagePacketData& data) const {
            size_t basisSize = 0;
            const auto serializedBasis = basisHandler->serializeCommonData(
                data,
                basisSize
            );

            validateBufferAllocations(buffer, basisSize);
            std::copy_n(
                serializedBasis.get(),
                basisSize,
                buffer->begin()
            );
        }

        void appendMultiByteField(
            std::vector<char>* buffer,
            const size_t startIndex,
            const size_t byteCount,
            const char* data
        ) const {
            const size_t requiredSize = startIndex + byteCount;
            validateBufferAllocations(buffer, requiredSize);

            std::copy_n(
                data,
                byteCount,
                buffer->begin() + static_cast<long>(startIndex)
            );
        }
    public:
        BinaryMessageHandler()
            : basisHandler(new version1::MessageBasisHandler{})
        {}

        ~BinaryMessageHandler() override {
            delete basisHandler;
        }
    };
} // namespace smart_home::usp_protocol::version1