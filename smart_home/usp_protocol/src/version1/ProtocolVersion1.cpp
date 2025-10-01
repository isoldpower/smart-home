#include "../../include/version1/ProtocolVersion1.h"

#include <iostream>

#include "../../include/version1/MessageBasisHandler.h"


namespace smart_home::usp_protocol::version1 {

    bool ProtocolVersion1::isMessageOfThisVersion(const char* message) const {
        const uint8_t targetVersion = getProtocolVersion(
            usp_protocol::ProtocolVersion::VERSION_1
        );

        return message != nullptr && message[0] == targetVersion;
    }

    std::unique_ptr<model::ProtocolBasisHandler> ProtocolVersion1::buildRelatedHandler() {
        return std::make_unique<version1::MessageBasisHandler>();
    }
} // namespace smart_home::usp_protocol::version1
