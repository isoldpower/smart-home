#pragma once

#include "../model/ProtocolBasisHandler.h"
#include "../model/ProtocolVersion.h"
#include "./CommonMessageData.h"


namespace smart_home::usp_protocol::version1 {

    class ProtocolVersion1 : public model::ProtocolVersion<char, CommonMessagePacketData> {
    public:
        bool isMessageOfThisVersion(const char* message) const override;
        std::unique_ptr<
            model::ProtocolBasisHandler<CommonMessagePacketData>
        > buildRelatedHandler() override;
    };
} // namespace smart_home::usp_protocol::version1