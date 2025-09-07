#pragma once

#include <smart_home/utilities/include/patterns/PresentableCommand.h>


namespace smart_home::playground::commands {

    class ProtocolSerializationCommand : public utilities::patterns::PresentableCommand<int> {
    public:
        int execute() override;
        void present() const override;
    };
} // namespace smart_home::playground::commands