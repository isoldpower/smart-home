#pragma once

#include "./messages/RequestMessage.h"


namespace smart_home::usp_protocol {

    class UspServerResponse : messages::RequestMessageData {
    public:
        UspServerResponse();
    };

} // namespace smart_home::usp_protocol