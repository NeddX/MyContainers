#ifndef MY_PAIR_H
#define MY_PAIR_H

#include <iostream>
#include <utility>

namespace my {
    template <typename T, typename U>
    struct Pair
    {
        T first;
        U second;

    public:
        template <typename... TArgs, typename... UArgs>
        static Pair<T, U> New(TArgs&&... targs, UArgs&&... uargs) noexcept
        {
            return Pair{ T(std::forward<TArgs>(targs)...), U(std::forward<UArgs>(uargs)...) };
        }
        static Pair<T, U> New(const T& first, const U& second) noexcept { return Pair{ first, second }; }

    public:
        friend std::ostream& operator<<(std::ostream& stream, const my::Pair<T, U>& pair) noexcept
        {
            stream << "{ " << pair.first << ", " << pair.second << " }";
            return stream;
        }
    };
} // namespace my

#endif // MY_PAIR_H
