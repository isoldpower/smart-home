#pragma once

#include <array>
#include <string>

#include "../../model/Message.h"
#include "../BinaryMessage.h"


namespace smart_home::usp_protocol::handlers::version1 {

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

    inline constexpr int ACKNOWLEDGEMENT_CHUNKS_AMOUNT = static_cast<size_t>(
        AcknowledgementSegmentsIndex::internal_Count
    );

    using AcknowledgementMessageRaw = BinaryMessageRaw<ACKNOWLEDGEMENT_CHUNKS_AMOUNT>;

    enum class AcknowledgementStatus : uint8_t {
        ACKNOWLEDGEMENT_SUCCESS = 0x00,
        ACKNOWLEDGEMENT_ERROR_TIMEOUT = 0x01,
        ACKNOWLEDGEMENT_ERROR_NOACK = 0x02,
        ACKNOWLEDGEMENT_UNKNOWN = 0xFF,
    };

    struct AcknowledgementMessage final : public model::Message {
    public:
        AcknowledgementStatus status;
        size_t size;
        std::string data;

        explicit AcknowledgementMessage(
            const ProtocolVersion& protocolVersion,
            const uint16_t& sessionId,
            const time_t& timestamp,
            const uint16_t& requestId,
            const AcknowledgementStatus& status,
            const size_t& size = 0,
            std::string data = ""
        );

        [[nodiscard]] bool isValid() const override;
    };

} // namespace smart_home::usp_protocol::handlers::version1