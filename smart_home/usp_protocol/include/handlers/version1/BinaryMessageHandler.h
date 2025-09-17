#pragma once

#include "../model/ProtocolMessageHandler.h"


namespace smart_home::usp_protocol::handlers::version1 {

    template <typename MessageType>
    class BinaryMessageHandler
        : public model::ProtocolMessageHandler<std::vector<char>, MessageType>
    {};

} // namespace smart_home::usp_protocol::handlers::version1