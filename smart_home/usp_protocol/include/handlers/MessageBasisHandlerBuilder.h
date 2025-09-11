#pragma once

#include <memory>

#include "./model/ProtocolBasisHandler.h"


namespace smart_home::usp_protocol::handlers {

    class MessageBasisHandlerBuilder {
    public:
        static std::unique_ptr<model::ProtocolBasisHandler> buildFromVersion(const char* buffer);
    };
}