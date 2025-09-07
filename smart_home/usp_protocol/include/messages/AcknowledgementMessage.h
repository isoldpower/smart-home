#pragma once

#include <array>
#include <string>
#include <utility>

#include "./BinaryMessage.h"
#include "./model/Message.h"


namespace smart_home::usp_protocol::messages {

    enum class AcknowledgementSegmentsIndex : size_t {
        VERSION = 0,
        SESSION_ID = 1,
        MESSAGE_TYPE = 2,
        TIMESTAMP = 3,
        REQUEST_ID = 4,
        STATUS = 5,
        SIZE = 6,
        DATA = 7,

        internal_Count = 8
    };

    inline constexpr int ACKNOWLEDGEMENT_CHUNKS_AMOUNT = static_cast<size_t>(AcknowledgementSegmentsIndex::internal_Count);

    using AcknowledgementMessageRaw = BinaryMessageRaw<ACKNOWLEDGEMENT_CHUNKS_AMOUNT>;

    enum class AcknowledgementStatus : uint8_t {
        ACKNOWLEDGEMENT_SUCCESS = 0x00,
        ACKNOWLEDGEMENT_ERROR_TIMEOUT = 0x01,
        ACKNOWLEDGEMENT_ERROR_NOACK = 0x02,
        ACKNOWLEDGEMENT_UNKNOWN = 0xFF,
    };

    struct AcknowledgementMessage final : public model::Message {
    public:
        std::string requestId;
        AcknowledgementStatus status;
        size_t size;
        std::string data;

        explicit AcknowledgementMessage(
            const ProtocolVersion& protocolVersion,
            std::string sessionId,
            const time_t& timestamp,
            std::string requestId,
            const AcknowledgementStatus& status,
            const size_t& size,
            std::string data
        )
            : Message(protocolVersion, std::move(sessionId), MessageType::MESSAGE_ACKNOWLEDGEMENT, timestamp)
            , requestId(std::move(requestId))
            , status(status)
            , size(size)
            , data(std::move(data))
        {}

        [[nodiscard]] bool isValid() const override;
    };

} // namespace smart_home::usp_protocol::messages