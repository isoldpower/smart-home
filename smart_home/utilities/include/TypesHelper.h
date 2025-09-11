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
} // namespace smart_home::utilities