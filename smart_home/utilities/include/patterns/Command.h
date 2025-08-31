#pragma once

namespace smart_home::utilities::patterns {

    template <typename T = void>
    class Command {
    public:
        virtual T execute() = 0;
        virtual ~Command() = default;
    };
}