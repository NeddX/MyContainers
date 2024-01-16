#ifndef MY_HASH_MAP_H
#define MY_HASH_MAP_H

#include <concepts>
#include <cstdint>
#include <functional>
#include <iostream>
#include <unordered_set>

#include <CommonDef.h>

#include "../ForwardList/ForwardList.h"
#include "../Vector/Vector.h"
#include "Utilities/Pair.h"

namespace my {

    // A type restriction that checks if T has implemented the equals operator or not
    // by simply checking the result of T == T.
    // If the operator is implemented than the result will a bool.
    template <typename T>
    concept HasEqualsOperator = requires(T a, T b) {
        {
            a == b
        } -> std::same_as<bool>;
    };

    // Another type restriction that checks if T is specialized for std::hash<T>.
    template <typename T>
    concept HasHashFunction = requires { typename std::hash<T>; };

    template <typename T, typename U>
        requires HasEqualsOperator<T> && HasHashFunction<T>
    class HashMap
    {
    public:
        static constexpr usize InitialBucketSize = 3;

    private:
        my::Vec<my::ForwardList<my::Pair<T, U>>> m_Table{};
        usize                                    m_Size        = HashMap<T, U>::InitialBucketSize;
        usize                                    m_MaxListSize = 0;

    public:
        HashMap();

    public:
        bool                  Insert(const T& key, const U& value);
        U&                    At(const T& key);
        bool                  Erase(const T& key);
        bool                  ContainsKey(const T& key) const noexcept;
        bool                  ContainsValue(const U& value) const noexcept;
        usize                 Size() const noexcept;
        bool                  Empty() const noexcept;
        void                  Clear() noexcept;
        my::Vec<U>            Values() const noexcept;
        std::unordered_set<T> KeySet() const noexcept;
        std::unordered_set<U> ValueSet() const noexcept;

    public:
        friend std::ostream& operator<<(std::ostream& stream, const my::HashMap<T, U>& map) noexcept
        {
            auto it = map.m_Table.cbegin();
            stream << "{ ";
            do
            {
                if (it == map.m_Table.cend())
                    break;

                for (const auto& e : *it)
                    stream << e;

                if (it + 1 != map.m_Table.cend())
                    stream << ", ";

                ++it;
            } while (true);
            stream << " }";
            return stream;
        }
    };
} // namespace my

#include "HashMap.hpp"
#endif // MY_HASH_MAP_H
