#pragma once

#include <chrono>


namespace smart_home::usp_protocol::handlers {

    template <typename T>
    concept HasRequestId = requires(T a) {
        { a.requestId } -> std::convertible_to<std::string>;
    };

    enum PendingRequestStatus {
        PENDING_REQUEST_STATUS_PENDING,
        PENDING_REQUEST_STATUS_COMPLETED,
        PENDING_REQUEST_STATUS_FAILED,
        PENDING_REQUEST_STATUS_TIMED_OUT
    };

    template <typename T>
    class PendingRequest {
        static_assert(
            HasRequestId<T>,
            "PendingRequest template parameter T must have a 'requestId' member of type std::string."
        );
    public:
        T request;
        std::time_t sentAt;
        size_t retries;
        PendingRequestStatus status;

        explicit PendingRequest(
            const T& request,
            const std::time_t& sentAt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
            const size_t& retries = 0
        )
            : request(request)
            , sentAt(sentAt)
            , retries(retries)
            , status(PENDING_REQUEST_STATUS_PENDING)
        {}

        ~PendingRequest() {

        }
    };
} // namespace smart_home::usp_protocol::handlers