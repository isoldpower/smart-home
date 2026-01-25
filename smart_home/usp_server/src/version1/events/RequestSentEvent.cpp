#include "../../../include/version1/events/RequestSentEvent.h"


namespace smart_home::usp_server::version1::events {

    std::string RequestSentEvent::getType() const {
        return getEventName(ServerEvent::RequestSentEvent);
    }
} // namespace smart_home::usp_server::version1::events