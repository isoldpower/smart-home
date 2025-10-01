#pragma once

#include <memory>

#include "../CommonMessageData.h"


namespace smart_home::usp_protocol::model {

    class ProtocolBasisHandler {
    public:
        virtual ~ProtocolBasisHandler() = default;
        virtual CommonMessageData parseCommonData(const char* buffer, size_t length) const = 0;
        virtual std::unique_ptr<char[]> serializeCommonData(
            const CommonMessageData& data,
            size_t& outSize
        ) const = 0;
    };

} // namespace smart_home::usp_protocol::model