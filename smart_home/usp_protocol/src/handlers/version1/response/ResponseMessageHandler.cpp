#include "../../../../include/handlers/version1/response/ResponseMessageHandler.h"


namespace smart_home::usp_protocol::handlers::version1 {

    std::unique_ptr<ResponseSerializationResult>
        ResponseMessageHandler::serialize(ResponseMessage* message)
    {
        // TODO: Implement proper serialization logic
        char resultBuffer[] = "Response Example";
        ResponseSerializationResult result {
            true,
            message,
            std::vector(
                resultBuffer,
                resultBuffer + sizeof(resultBuffer) / sizeof(char)
            )
        };

        return std::make_unique<ResponseSerializationResult>(result);
    }

    std::unique_ptr<ResponseDeserializationResult>
        ResponseMessageHandler::deserialize(std::vector<char>* buffer)
    {
        // TODO: Implement proper deserialization logic
        ResponseMessage resultMessage {
            ProtocolVersion::VERSION_1,
            8823,
            time(nullptr),
            22233,
            1,
            0,
            ResponseStatus::STATUS_OK,
            buffer->size(),
            buffer->data()
        };
        model::DeserializationResult result { true, buffer, resultMessage };

        return std::make_unique<ResponseDeserializationResult>(result);
    }
} // namespace smart_home::usp_protocol::handlers::version1