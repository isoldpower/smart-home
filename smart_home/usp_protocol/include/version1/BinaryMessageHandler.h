#pragma once

#include <vector>

#include "../model/ProtocolMessageHandler.h"
#include "./MessageBasisHandler.h"


namespace smart_home::usp_protocol::version1 {

    template <typename MessageType>
    class BinaryMessageHandler
        : public model::ProtocolMessageHandler<std::vector<char>, MessageType> {
    protected:
        MessageBasisHandler* basisHandler;

        void appendBasis(std::vector<char>* buffer, const CommonMessageData& data) const {
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

        void appendMultiByteField(
            std::vector<char>* buffer,
            const size_t startIndex,
            const size_t byteCount,
            const char* data
        ) const {
            const size_t requiredSize = startIndex + byteCount;

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