#include "../../include/handlers/MessageBasisHandlerBuilder.h"

#include <iostream>

#include "../../include/handlers/CommonMessageData.h"
#include "../../include/handlers/version1/MessageBasisHandler.h"


namespace smart_home::usp_protocol::handlers {

    std::unique_ptr<model::ProtocolBasisHandler> MessageBasisHandlerBuilder::buildFromVersion(const char* buffer) {
        if (buffer[0] == '\0') {
            std::cerr << "Empty buffer provided for protocol version detection" << std::endl;
            return nullptr;
        }

        const char versionByte = buffer[0];
        switch (static_cast<ProtocolVersion>(versionByte)) {
            case ProtocolVersion::VERSION_1:
                return std::make_unique<version1::ProtocolMessageBasisHandler>();
            default:
                std::cerr << "Unknown protocol version detected: " << std::to_string(versionByte) << std::endl;
                return nullptr;
        }
    }
} // namespace smart_home::usp_protocol::handlers