#ifndef MY_UTILITIES_MATH_H
#define MY_UTILITIES_MATH_H

#include <cstdint>

#ifndef RUSTY_TYPE_ALIASES
#define RUSTY_TYPE_ALIASES
using usize   = std::size_t;
using ptrdiff = std::ptrdiff_t;
using intptr  = std::intptr_t;
using uintptr = std::uintptr_t;
using u8      = std::uint8_t;
using i8      = std::int8_t;
using u16     = std::uint16_t;
using i16     = std::int16_t;
using u32     = std::uint32_t;
using i32     = std::int32_t;
using u64     = std::uint64_t;
using i64     = std::int64_t;
using f32     = float;
using f128    = long double;
#endif // RUSTY_TYPE_ALIASES

namespace my {
    bool  IsPrime(const usize num) noexcept;
    usize NextPrime(const usize num) noexcept;
} // namespace my

#endif // MY_UTILITIES_MATH_H
