#pragma once

#include <iostream>
#include <map>
#include <ostream>
#include <string>


namespace smart_home::usp_server::version1::events {

    enum class ServerEvent {
        MessageReceivedEvent,
        RequestSentEvent,

        AnonymousEvent,
    };

    class ServerEvents {
    public:
        inline static const std::map<ServerEvent, std::string> EventsList {
            { ServerEvent::MessageReceivedEvent, "MessageReceivedEvent" },
            { ServerEvent::AnonymousEvent, "AnonymousEvent" }
        };
    };

    inline std::string getEventName(const ServerEvent event) {
        if (ServerEvents::EventsList.contains(event)) {
            return ServerEvents::EventsList.at(event);
        }

        std::cerr << "Event not found error" << std::endl;
        if (!ServerEvents::EventsList.contains(ServerEvent::AnonymousEvent)) {
            return "AnonymousEvent";
        } else {
            return getEventName(ServerEvent::AnonymousEvent);
        }
    }

} // namespace smart_home::usp_server::version1::events