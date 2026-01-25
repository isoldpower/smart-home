#pragma once

#include <smart_home/utilities/include/patterns/EventChannel.h>

#include <utility>

#include "./ServerEvents.h"
#include "../ReferencedCommonData.h"


namespace smart_home::usp_server::version1::events {

    class MessageReceivedEvent : public utilities::patterns::Event {
    public:
        std::vector<std::shared_ptr<ReferencedCommonData>> packetsList;

        explicit MessageReceivedEvent(
            std::vector<std::shared_ptr<ReferencedCommonData>> packetsList
        )
            : Event()
            , packetsList(std::move(packetsList))
        {}
        ~MessageReceivedEvent() override = default;

        [[nodiscard]] std::string getType() const override;
    };
} // namespace smart_home::usp_server::version1::events