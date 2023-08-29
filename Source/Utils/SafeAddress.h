#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <type_traits>

#include "ReturnAddress.h"

class SafeAddress {
public:
    explicit SafeAddress(std::uintptr_t address)
        : address{ address }
    {
    }

    SafeAddress& add(std::ptrdiff_t offset) noexcept
    {
        if (address != 0)
            address += offset;
        return *this;
    }

    template <std::size_t N>
    SafeAddress& deref() noexcept
    {
        if constexpr (N != 0) {
            if (address != 0) {
                address = *reinterpret_cast<std::uintptr_t*>(address);
                return deref<N - 1>();
            }
        }
        return *this;
    }

    SafeAddress& deref() noexcept
    {
        return deref<1>();
    }

    SafeAddress& abs() noexcept
    {
        if (address != 0) {
            using OffsetType = std::int32_t;
            const auto addressOfNextInstruction = address + sizeof(OffsetType);
            OffsetType offset;
            std::memcpy(&offset, reinterpret_cast<const void*>(address), sizeof(OffsetType));
            address = addressOfNextInstruction + offset;
        }
        return *this;
    }

    [[nodiscard]] std::uintptr_t get() const noexcept
    {
        return address;
    }

    [[nodiscard]] ReturnAddress asReturnAddress() const noexcept
    {
        return ReturnAddress{ address };
    }

    template <typename T>
    [[nodiscard]] T as() const noexcept
    {
        static_assert(std::is_pointer_v<T>, "T must be a pointer type!");
        return reinterpret_cast<T>(address);
    }

private:
    std::uintptr_t address;
};
