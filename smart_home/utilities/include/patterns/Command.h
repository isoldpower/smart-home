#pragma once

namespace smart_home::utilities::patterns {

    template <typename T = void>
    class Command {
    public:
        virtual T execute(int argc, char* argv[]) = 0;
        virtual ~Command() = default;
    };
}