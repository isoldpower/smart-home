#pragma once

#include <memory>

#include "./ProtocolBasisHandler.h"


namespace smart_home::usp_protocol::model {

    template <typename T>
    class ProtocolVersion {
    public:
        virtual ~ProtocolVersion() = default;

        virtual bool isMessageOfThisVersion(const T* message) const = 0;
        virtual std::unique_ptr<ProtocolBasisHandler> buildRelatedHandler() = 0;
    };

} // namespace smart_home::usp_protocol::model