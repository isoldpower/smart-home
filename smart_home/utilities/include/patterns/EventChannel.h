#pragma once

#include <any>
#include <functional>
#include <map>
#include <algorithm>


namespace smart_home::utilities::patterns {

    class Event {
    public:
        virtual ~Event() = default;

        [[nodiscard]] virtual std::string getType() const = 0;
    };

    class EventChannel {
    private:
        std::multimap<std::string, std::function<void(const Event&)>> handlers;
    public:
        EventChannel()
            : handlers({})
        {}

        template <typename T>
        void subscribe(
            const std::string eventType,
            const std::function<std::any(const T&)>& callback
        ) {
            handlers.emplace(eventType, [callback](const Event& event) {
                callback(static_cast<const T&>(event));
            });
        }

        void dispatch(const Event& event) const {
            const std::string eventType = event.getType();
            auto [begin, end]  = handlers.equal_range(eventType);

            std::for_each(
                begin,
                end,
                [&](const std::pair<
                    std::string,
                    std::function<void(const Event&)>
                >& callback) {
                    callback.second(event);
                }
            );
        }
    };
} // namespace smart_home::utilities::patterns