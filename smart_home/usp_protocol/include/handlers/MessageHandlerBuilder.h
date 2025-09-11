#pragma once

#include <memory>
#include <iostream>

#include "./model/ProtocolBasisHandler.h"
#include "./version1/MessageBasisHandler.h"


namespace smart_home::usp_protocol::handlers {

    class MessageBasisHandlerBuilder {
    public:
        static std::unique_ptr<model::ProtocolBasisHandler> buildFromVersion(const char* buffer) {
            if (buffer[0] == '\0') {
                std::cerr << "Empty buffer provided for protocol version detection" << std::endl;
                return nullptr;
            }

            const char versionByte = buffer[0];
            switch (static_cast<messages::ProtocolVersion>(versionByte)) {
                case messages::ProtocolVersion::VERSION_1:
                    return std::make_unique<version1::ProtocolMessageBasisHandler>();
                default:
                    std::cerr << "Unknown protocol version detected: " << std::to_string(versionByte) << std::endl;
                    return nullptr;
            }
        }
    };
}