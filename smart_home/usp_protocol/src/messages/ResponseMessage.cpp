#include "../../include/messages/ResponseMessage.h"


namespace smart_home::usp_protocol::messages {

    bool ResponseMessage::isValid() const {
        return true;
    }
} // namespace smart_home::usp_protocol::messages