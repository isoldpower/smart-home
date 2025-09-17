#include "../../../../include/handlers/version1/request/RequestMessageHandler.h"


namespace smart_home::usp_protocol::handlers::version1 {

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
        // TODO: Implement proper deserialization logic
        RequestMessage resultMessage {
            ProtocolVersion::VERSION_1,
            21312,
            time(nullptr),
            39933,
            1,
            0,
            "",
            0x01,
            0x01,
            buffer->size(),
            buffer->data()
        };
        model::DeserializationResult result { true, buffer, resultMessage };

        return std::make_unique<RequestDeserializationResult>(result);
    }
} // namespace smart_home::usp_protocol::handlers::version1