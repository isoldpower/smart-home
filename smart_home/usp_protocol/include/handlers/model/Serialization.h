#pragma once

#include <memory>


namespace smart_home::usp_protocol::handlers::model {

    template <typename TMessage, typename TSerialized>
    struct SerializationResult {
    private:
        TMessage* referencedMessage = nullptr;
        TSerialized serializationState;
        bool success = false;
    public:
        SerializationResult(bool success, TMessage* message, TSerialized buffer)
            : referencedMessage(message)
            , serializationState(std::move(buffer))
            , success(success)
        {}

        [[nodiscard]] TSerialized* getSerializationState() {
            return &serializationState;
        }

        [[nodiscard]] bool getIsSuccess() const {
            return success;
        }
    };

    template <typename TMessage, typename TSerialized>
    class ISerializable {
    public:
        virtual ~ISerializable() = default;

        virtual std::unique_ptr<SerializationResult<TMessage, TSerialized>>
            serialize(TMessage*) = 0;
    };
} // namespace smart_home::usp_protocol::handlers::model