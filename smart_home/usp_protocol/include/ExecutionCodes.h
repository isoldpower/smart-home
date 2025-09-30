#pragma once

namespace smart_home::usp_protocol {

    enum class ExecutionCodes : int {
        RECEIVED_PACKAGE_SIZE_ERROR = -101,
        RECEIVED_PACKAGE_DATA_ERROR = -102,
        RECEIVED_PACKAGE_UNKNOWN_VERSION = -103,
        RECEIVED_PACKAGE_UNKNOWN_TYPE = -104,
        RECEIVED_PACKAGE_UNKNOWN_ACTION = -105,
        WRONG_VERSION_STRUCTURE = -106,

        INEFFICIENT_MEMORY_ALLOCATION = -201,
    };

    consteval int castedExecutionCode(ExecutionCodes code) {
        return static_cast<int>(code);
    }
} // namespace smart_home::usp_protocol