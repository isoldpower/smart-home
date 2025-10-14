#pragma once


namespace smart_home::usp_server::exceptions {

    enum class ExecutionCodes {
        UNKNOWN_MESSAGE_TYPE = -101,
    };

    consteval int castedExecutionCode(ExecutionCodes code) {
        return static_cast<int>(code);
    }

    constexpr int castExecutionCode(ExecutionCodes code) {
        return static_cast<int>(code);
    }
} // namespace smart_home::usp_server