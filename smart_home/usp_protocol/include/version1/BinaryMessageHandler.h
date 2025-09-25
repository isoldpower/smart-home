#pragma once

#include <vector>

#include "../model/ProtocolMessageHandler.h"
#include "./MessageBasisHandler.h"


namespace smart_home::usp_protocol::version1 {

    template <typename MessageType>
    class BinaryMessageHandler
        : public model::ProtocolMessageHandler<std::vector<char>, MessageType> {
    protected:
        MessageBasisHandler* basisHandler;
    public:
        BinaryMessageHandler()
            : basisHandler(new version1::MessageBasisHandler{})
        {}

        ~BinaryMessageHandler() override {
            delete basisHandler;
        }
    };

} // namespace smart_home::usp_protocol::version1