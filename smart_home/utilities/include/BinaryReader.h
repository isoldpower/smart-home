#pragma once

#include <stdexcept>
#include <string>
#include <type_traits>


namespace smart_home::utilities {

    template <typename T>
    concept UintDerivative = std::is_same_v<uint8_t, std::remove_cvref_t<T>> ||
                             std::is_same_v<uint16_t, std::remove_cvref_t<T>> ||
                             std::is_same_v<uint32_t, std::remove_cvref_t<T>> ||
                             std::is_same_v<uint64_t, std::remove_cvref_t<T>>;

    class BigEndianReader {
    public:
        template <UintDerivative T>
        static T bytesToUint(const char* bytes, size_t length = sizeof(T)) {
            constexpr size_t expectedLength = sizeof(T);
            if (bytes == nullptr) {
                throw std::invalid_argument("Null-pointer buffer provided");
            } else if (length < expectedLength) {
                throw std::invalid_argument(
                    "Insufficient bytes for conversion to " +
                    std::string(typeid(T).name()) +
                    "; expected " + std::to_string(expectedLength) +
                    ", got " + std::to_string(length));
            }

            T result = 0;
            for (size_t i = 0; i < expectedLength; ++i) {
                // Shift the current result left by 8 bits (1 byte) and add the next byte;
                // This ensures big-endian interpretation
                result = (result << 8) | (static_cast<uint8_t>(bytes[i]));
            }

            return result;
        }

        // Helper functions for specific sizes
        static uint8_t bytesToUint8(const char bytes[1]) {
            return bytesToUint<uint8_t>(bytes, 1);
        }

        static uint16_t bytesToUint16(const char bytes[2]) {
            return bytesToUint<uint16_t>(bytes, 2);
        }

        static uint32_t bytesToUint32(const char bytes[4]) {
            return bytesToUint<uint32_t>(bytes, 4);
        }

        static uint64_t bytesToUint64(const char bytes[8]) {
            return bytesToUint<uint64_t>(bytes, 8);
        }

        template <UintDerivative T>
        static std::unique_ptr<char[]> uintToBytes(T value, const size_t length) {
            constexpr size_t expectedLength = sizeof(T);
            if (length < expectedLength) {
                throw std::invalid_argument(
                    "Insufficient bytes for conversion to " +
                    std::string(typeid(T).name()) +
                    "; expected " + std::to_string(expectedLength) +
                    ", got " + std::to_string(length));
            }

            auto bytes = std::make_unique<char[]>(length);
            for (size_t i = 0; i < length; ++i) {
                // Store bytes in big-endian order. We extract the least significant byte first
                // and place it at the end of the array, moving backwards.
                bytes[length - 1 - i] = static_cast<char>(value & 0xFF);
                value >>= 8;
            }

            return bytes;
        }

        // Helper functions for specific sizes
        static std::unique_ptr<char[]> uint8ToBytes(const uint8_t value) {
            return uintToBytes(value, 1);
        }

        static std::unique_ptr<char[]> uint16ToBytes(const uint16_t value) {
            return uintToBytes(value, 2);
        }

        static std::unique_ptr<char[]> uint32ToBytes(const uint32_t value) {
            return uintToBytes(value, 4);
        }

        static std::unique_ptr<char[]> uint64ToBytes(const uint64_t value) {
            return uintToBytes(value, 8);
        }
    };

} // namespace smart_home::utilities