#pragma once

#include <memory>


namespace smart_home::usp_protocol::model {

    template <typename T>
    class ProtocolBasisHandler {
    public:
        virtual ~ProtocolBasisHandler() = default;
        virtual T parseCommonData(
            const char* buffer,
            size_t length
        ) const = 0;
        virtual std::unique_ptr<char[]> serializeCommonData(
            const T& data,
            size_t& outSize
        ) const = 0;
    };

} // namespace smart_home::usp_protocol::model