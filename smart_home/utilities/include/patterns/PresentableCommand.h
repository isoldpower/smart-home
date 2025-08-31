#pragma once
#include "Command.h"

namespace smart_home::utilities::patterns {
    template <typename T = void>
    class PresentableCommand : public Command<T> {
    public:
        virtual void present() const = 0;
    };
} // namespace smart_home::utilities::patterns