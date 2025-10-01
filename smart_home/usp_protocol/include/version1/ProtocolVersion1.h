#pragma once

#include "../model/ProtocolBasisHandler.h"
#include "../model/ProtocolVersion.h"


namespace smart_home::usp_protocol::version1 {

    class ProtocolVersion1 : public model::ProtocolVersion<char> {
    public:
        bool isMessageOfThisVersion(const char* message) const override;
        std::unique_ptr<model::ProtocolBasisHandler> buildRelatedHandler() override;
    };
} // namespace smart_home::usp_protocol::version1