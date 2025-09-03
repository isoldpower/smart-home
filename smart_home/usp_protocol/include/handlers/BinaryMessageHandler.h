#pragma once

#include "./model/ProtocolMessageHandler.h"
#include "../messages/BinaryMessage.h"


namespace smart_home::usp_protocol::handlers {

    template <typename MessageType, size_t ChunksAmount>
    class BinaryMessageHandler
        : public model::ProtocolMessageHandler<messages::BinaryMessageRaw<ChunksAmount>, MessageType>
    {};

} // namespace smart_home::usp_protocol::handlers