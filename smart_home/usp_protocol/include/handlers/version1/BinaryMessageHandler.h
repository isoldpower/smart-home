#pragma once

#include "../model/ProtocolMessageHandler.h"
#include "BinaryMessage.h"


namespace smart_home::usp_protocol::handlers::version1 {

    template <typename MessageType, size_t ChunksAmount>
    class BinaryMessageHandler
        : public model::ProtocolMessageHandler<BinaryMessageRaw<ChunksAmount>, MessageType>
    {};

} // namespace smart_home::usp_protocol::handlers::version1