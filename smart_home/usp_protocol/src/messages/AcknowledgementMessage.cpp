#include "../../include/messages/AcknowledgementMessage.h"


namespace smart_home::usp_protocol::messages {

    bool AcknowledgementMessage::isValid() const {
        return true;
    }
} // namespace smart_home::usp_protocol::messages