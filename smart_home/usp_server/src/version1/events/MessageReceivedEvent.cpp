#include "../../../include/version1/events/MessageReceivedEvent.h"


namespace smart_home::usp_server::version1::events {

    std::string MessageReceivedEvent::getType() const {
        return getEventName(ServerEvent::MessageReceivedEvent);
    }
} // namespace smart_home::usp_server::version1::events