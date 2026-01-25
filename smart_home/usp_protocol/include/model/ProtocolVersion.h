#pragma once

#include <memory>

#include "./ProtocolBasisHandler.h"


namespace smart_home::usp_protocol::model {

    template <typename T, typename U>
    class ProtocolVersion {
    public:
        virtual ~ProtocolVersion() = default;

        virtual bool isMessageOfThisVersion(const T* message) const = 0;
        virtual std::unique_ptr<ProtocolBasisHandler<U>> buildRelatedHandler() = 0;
    };
} // namespace smart_home::usp_protocol::model