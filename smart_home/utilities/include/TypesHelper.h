#pragma once

#include <functional>


namespace smart_home::utilities {

    template <typename TClass, typename TReturn, typename... TArgs>
    static std::function<TReturn(TArgs...)> methodToFunction(TReturn (TClass::*method)(TArgs...),
                                                             TClass *instance) {
        return [method, instance](TArgs... args) -> TReturn {
            return (instance->*method)(args...);
        };
    }

    template <typename TClass, typename TReturn, typename... TArgs>
    static std::function<TReturn(TArgs...)> methodToFunction(TReturn (TClass::*method)(TArgs...) const,
                                                            const TClass* instance) {
        return [method, instance](TArgs... args) -> TReturn {
            return (instance->*method)(args...);
        };
    }

    inline uint16_t bytesToUint16(const char bytes[2]) {
        return (static_cast<uint16_t>(bytes[0]) << 8) | static_cast<uint16_t>(bytes[1]);
    }

    inline uint32_t bytesToUint32(const char bytes[4]) {
        return (static_cast<uint32_t>(bytes[0]) << 24) |
               (static_cast<uint32_t>(bytes[1]) << 16) |
               (static_cast<uint32_t>(bytes[2]) << 8) |
               static_cast<uint32_t>(bytes[3]);
    }
} // namespace smart_home::utilities