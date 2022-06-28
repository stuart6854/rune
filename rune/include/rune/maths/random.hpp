#pragma once

#include "rune/defines.hpp"

namespace Rune
{
    namespace Random
    {
        /**
         * @return Random float value in range [0..1]
         */
        auto valueFloat() -> f32;

        /**
         * @return Random double value in range [0..1]
         */
        auto valueDouble() -> f64;

        /**
         * @return Random 32bit integer value in range [min, max]
         */
        auto rangeInt(i32 min, i32 max) -> i32;

        /**
         * @return Random 64bit integer value in range [min, max]
         */
        auto rangeLong(i64 min, i64 max) -> i64;

        /**
         * @return Random 32bit integer value in range [0, Max]
         */
        auto randomInt() -> i32;

        /**
         * @return Random 64bit integer value in range [0, Max]
         */
        auto randomLong() -> i64;

        /**
         * @return Random 32bit float value in range [min, max]
         */
        auto rangeFloat(f32 min, f32 max) -> f32;

        /**
         * @return Random 64bit double value in range [min, max]
         */
        auto rangeDouble(f64 min, f64 max) -> f64;

        /**
         * @return Random 32bit float value in range [0, Max]
         */
        auto randomFloat() -> f32;

        /**
         * @return Random 64bit double value in range [0, Max]
         */
        auto randomDouble() -> f64;

        /**
         * @return Random 32bit unsigned integer value in range [min, max]
         */
        auto rangeUnsignedInt(u32 min, u32 max) -> u32;

        /**
         * @return Random 64bit unsigned integer value in range [min, max]
         */
        auto rangeUnsignedLong(u64 min, u64 max) -> u64;

        /**
         * @return Random 32bit unsigned integer value in range [0, Max]
         */
        auto randomUnsignedInt() -> u32;

        /**
         * @return Random 64bit unsigned integer value in range [0, Max]
         */
        auto randomUnsignedLong() -> u64;
    }
}