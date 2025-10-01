#pragma once

#include <memory>

#include "./model/ProtocolBasisHandler.h"


namespace smart_home::usp_protocol {

    class MessageHandlerBuilder {
    private:
        const char* buffer;
    public:
        explicit MessageHandlerBuilder(const char* buffer);

        [[nodiscard]] std::unique_ptr<model::ProtocolBasisHandler> buildBasisHandler() const;
    };
}