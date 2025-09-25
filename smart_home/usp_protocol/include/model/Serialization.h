#pragma once

#include <memory>
#include <string>


namespace smart_home::usp_protocol::model {

    template <typename TSerialized>
    concept SerializedType =
        std::ranges::range<TSerialized>
        && requires (TSerialized serialized) {
            { serialized.at(0) } -> std::convertible_to<std::string>;
        };

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
} // namespace smart_home::usp_protocol::model