#pragma once

#include <memory>


namespace smart_home::usp_protocol::model {

    template <typename TMessage, typename TSerialized>
        struct DeserializationResult {
    private:
        TSerialized* referencedBuffer;
        TMessage deserializationState = nullptr;
        bool success = false;
    public:
        DeserializationResult(bool success, TSerialized* buffer, TMessage message)
            : referencedBuffer(buffer)
            , deserializationState(std::move(message))
            , success(success)
        {}

        [[nodiscard]] TMessage* getDeserializationState() {
            return &deserializationState;
        }

        [[nodiscard]] bool isSuccess() const {
            return success;
        }
    };

    template <typename TMessage, typename TSerialized>
    class IDeserializable {
    public:
        virtual ~IDeserializable() = default;

        virtual std::unique_ptr<DeserializationResult<TMessage, TSerialized>>
            deserialize(TSerialized*) = 0;
    };
} // namespace smart_home::usp_protocol::model