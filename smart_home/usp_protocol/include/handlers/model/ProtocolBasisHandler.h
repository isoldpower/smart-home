#pragma once

#include "../CommonMessageData.h"

namespace smart_home::usp_protocol::handlers::model {

    class ProtocolBasisHandler {
    public:
        virtual ~ProtocolBasisHandler() = default;
        virtual CommonMessageData parseCommonData(const char* buffer, size_t length) const = 0;
    };

} // namespace smart_home::usp_protocol::handlers::model