#pragma once

namespace smart_home::usp_protocol::net {

    enum NetServerExitCodes : int {
        EXT_ERR_SOCKET_CREATION = 401,
        EXT_ERR_SOCKET_BINDING = 402,
        EXT_ERR_RECEIVE_MESSAGE = 403,
        EXT_ERR_SET_TIMEOUT = 404,
        LISTENING_FAILURE = 3,
        ACCEPTING_FAILURE = 4,
        RECEIVING_FAILURE = 5,
        SENDING_FAILURE = 6,
    };
} // namespace smart_home::usp_protocol::net