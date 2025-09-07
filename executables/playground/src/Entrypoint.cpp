#include "../include/Entrypoint.h"

#include <smart_home/utilities/include/patterns/PresentableCommand.h>
#include <any>
#include <iostream>

#include "../include/commands/DelayedDaemonCommand.h"
#include "../include/commands/ProtocolSerializationCommand.h"
#include "../include/commands/UspServerRequestCommand.h"


namespace smart_home::playground {
    using namespace smart_home::utilities::patterns;

    int readUserInput();

    template <typename T, size_t N>
    bool validateUserInput(const std::array<T, N>& commands, int commandIndex);

    int runPlaygroundEntrypoint() {
        const std::array<PresentableCommand<int>*, 3> commands = {
            new commands::DelayedDaemonCommand{},
            new commands::ProtocolSerializationCommand{},
            new commands::UspServerRequestCommand{}
        };

        for (size_t i = 0; i < commands.size(); i++) {
            std::cout << i + 1 << ") ";
            commands[i]->present();
        }

        int commandIndex = readUserInput();
        if (validateUserInput(commands, commandIndex)) {
            int result = commands[commandIndex]->execute();
            for (const auto& cmd : commands) {
                delete cmd;
            }

            return result;
        } else {
            std::cerr << "Invalid command index. Terminating the process!" << std::endl;
            return 1;
        }
    }

    int readUserInput() {
        std::string userInput;
        std::cin >> userInput;

        try {
            int commandIndex = std::stoi(userInput);
            return commandIndex - 1;
        } catch (std::invalid_argument& e) {
            std::cerr << "Expected command number, received string instead" << std::endl;
            return -1;
        }
    }

    template <typename T, size_t N>
    bool validateUserInput(const std::array<T, N>& commands, int commandIndex) {
        return commandIndex >= 0 && commandIndex < static_cast<int>(commands.size());
    }

} // namespace smart_home::playground