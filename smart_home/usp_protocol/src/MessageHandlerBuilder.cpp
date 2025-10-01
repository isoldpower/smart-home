#include "../include/MessageHandlerBuilder.h"

#include "../include/version1/MessageBasisHandler.h"
#include "../include/version1/ProtocolVersion1.h"

#include <iostream>
#include <array>


namespace smart_home::usp_protocol {

    MessageHandlerBuilder::MessageHandlerBuilder(const char* buffer)
        : buffer(buffer)
    {}

    std::unique_ptr<model::ProtocolBasisHandler> MessageHandlerBuilder::buildBasisHandler() const {
        if (buffer[0] == '\0') {
            std::cerr << "Empty buffer provided for protocol version detection" << std::endl;
            return nullptr;
        }

        std::unique_ptr<model::ProtocolBasisHandler> relatedVersion = nullptr;
        std::array<model::ProtocolVersion<char>*, 1> versions = {
            new version1::ProtocolVersion1{},
        };

        for (const auto& version : versions) {
            if (version->isMessageOfThisVersion(buffer)) {
                relatedVersion = version->buildRelatedHandler();
            }
            delete version;
        }

        if (relatedVersion == nullptr) {
            std::cerr << "Unable to resolve protocol version of passed buffer:" << std::endl
                      << '\t' << buffer << std::endl;
            return nullptr;
        } else {
            return relatedVersion;
        }
    }
} // namespace smart_home::usp_protocol