#pragma once

#include <smart_home/utilities/include/patterns/EventChannel.h>

#include "./ServerEvents.h"


namespace smart_home::usp_server::version1::events {

    class RequestSentEvent : public utilities::patterns::Event {
    public:
        ~RequestSentEvent() override = default;

        [[nodiscard]] std::string getType() const override;
    };
} // namespace smart_home::usp_server::version1::events
