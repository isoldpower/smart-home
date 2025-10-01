#include "../include/ExitCodes.h"

#include <cstdlib>


namespace smart_home::web_server {

    int exitWithCode(const NetServerExitCodes exitCode) {
        std::exit(static_cast<int>(exitCode));
    }
} // namespace smart_home::web_server