#include "rune/maths/random.hpp"

#include <random>

namespace Rune
{
    static std::mt19937 s_engine(std::random_device{}());

    auto Random::valueFloat() -> f32
    {
        std::uniform_real_distribution dist(0.0f, 1.0f);
        return dist(s_engine);
    }

    auto Random::valueDouble() -> f64
    {
        std::uniform_real_distribution dist(0.0, 1.0);
        return dist(s_engine);
    }

    auto Random::rangeInt(const i32 min, const i32 max) -> i32
    {
        std::uniform_int_distribution dist(min, max);
        return dist(s_engine);
    }

    auto Random::rangeLong(const i64 min, const i64 max) -> i64
    {
        std::uniform_int_distribution dist(min, max);
        return dist(s_engine);
    }

    auto Random::randomInt() -> i32
    {
        return rangeInt(0, i32_max);
    }

    auto Random::randomLong() -> i64
    {
        return rangeLong(0, i64_max);
    }

    auto Random::rangeFloat(const f32 min, const f32 max) -> f32
    {
        std::uniform_real_distribution dist(min, max);
        return dist(s_engine);
    }

    auto Random::rangeDouble(const f64 min, const f64 max) -> f64
    {
        std::uniform_real_distribution dist(min, max);
        return dist(s_engine);
    }

    auto Random::randomFloat() -> f32
    {
        return rangeFloat(0, f32_max);
    }

    auto Random::randomDouble() -> f64
    {
        return rangeDouble(0, f64_max);
    }

    auto Random::rangeUnsignedInt(const u32 min, const u32 max) -> u32
    {
        std::uniform_int_distribution dist(min, max);
        return dist(s_engine);
    }

    auto Random::rangeUnsignedLong(const u64 min, const u64 max) -> u64
    {
        std::uniform_int_distribution dist(min, max);
        return dist(s_engine);
    }

    auto Random::randomUnsignedInt() -> u32
    {
        return rangeUnsignedInt(0, u32_max);
    }

    auto Random::randomUnsignedLong() -> u64
    {
        return rangeUnsignedLong(0, u64_max);
    }

}
