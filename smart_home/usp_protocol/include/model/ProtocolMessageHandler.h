#pragma once

#include "./Serialization.h"
#include "./Deserialization.h"


namespace smart_home::usp_protocol::model {

    template <typename TSerialized, typename TMessage>
    class ProtocolMessageHandler
        : public ISerializable<TMessage, TSerialized>
        , public IDeserializable<TMessage, TSerialized>
    {};
} // namespace smart_home::usp_protocol::model