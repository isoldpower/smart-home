#include "../../include/commands/ProtocolSerializationCommand.h"

#include <iostream>
#include <ostream>


namespace smart_home::playground::commands {

    int ProtocolSerializationCommand::execute() {
        std::cout << "Protocol Serialization Playground" << std::endl;
        return 0;
    }

    void ProtocolSerializationCommand::present() const {
        std::cout << "Protocol Serialization Command: "
                     "Demonstrates serialization and deserialization process of a data with "
                     "a format that will be used to travel over the net " << std::endl;
    }
} // namespace smart_home::playground