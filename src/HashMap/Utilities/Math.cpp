#include "Math.h"

#include <limits>

namespace my {
    bool IsPrime(const usize num) noexcept
    {
        if (num <= 1)
            return false;

        for (usize i = 2; i < num; ++i)
            if (num % i == 0)
                return false;

        return true;
    }

    usize NextPrime(const usize num) noexcept
    {
        for (usize i = num + 1; i < std::numeric_limits<usize>::max() - num; ++i)
        {
            if (IsPrime(i))
                return i;
        }
        return num;
    }
} // namespace my
