#pragma once

#include <cstdint>
#include <limits>

// Types

typedef std::size_t size;

typedef std::int8_t i8;
typedef std::int16_t i16;
typedef std::int32_t i32;
typedef std::int64_t i64;

typedef std::uint8_t u8;
typedef std::uint16_t u16;
typedef std::uint32_t u32;
typedef std::uint64_t u64;

constexpr auto i8_max = std::numeric_limits<i8>::max();
constexpr auto i16_max = std::numeric_limits<i16>::max();
constexpr auto i32_max = std::numeric_limits<i32>::max();
constexpr auto i64_max = std::numeric_limits<i64>::max();

constexpr auto u8_max = std::numeric_limits<u8>::max();
constexpr auto u16_max = std::numeric_limits<u16>::max();
constexpr auto u32_max = std::numeric_limits<u32>::max();
constexpr auto u64_max = std::numeric_limits<u64>::max();

typedef float f32;
typedef double f64;

constexpr auto f32_max = std::numeric_limits<f32>::max();
constexpr auto f64_max = std::numeric_limits<f64>::max();

#include <memory>

namespace Rune
{
    template <typename T>
    using Owned = std::unique_ptr<T>;
    template <typename T, typename... Args>
    constexpr Owned<T> CreateOwned(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template <typename T>
    using Shared = std::shared_ptr<T>;
    template <typename T, typename... Args>
    constexpr Shared<T> CreateShared(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}
