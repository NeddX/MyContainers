#ifndef MY_HASH_MAP_IMPL_HPP
#define MY_HASH_MAP_IMPL_HPP

#include "Utilities/Math.h"

#define HASHMAP_TEMPLATE_DECL()                                                                                        \
    template <typename T, typename U>                                                                                  \
        requires HasEqualsOperator<T> && HasHashFunction<T>

namespace my {
    HASHMAP_TEMPLATE_DECL()
    HashMap<T, U>::HashMap() : m_Table(HashMap<T, U>::InitialBucketSize)
    {
    }

    HASHMAP_TEMPLATE_DECL()
    bool HashMap<T, U>::Insert(const T& key, const U& value)
    {
        const auto hash     = std::hash<T>{}(key);
        const auto hash_idx = hash % m_Size;

        // Resize and rehash if a any linked lists' size equals to our vector
		// or our hash is bigger than our current size.
        if (hash_idx >= m_Size || m_MaxListSize + 1 == m_Table.Size())
        {
            decltype(m_Table) copy{};
            copy = std::move(m_Table);

            m_Size  = NextPrime(m_Size);
            m_Table = decltype(m_Table)(m_Size);
            for (const auto& e : copy)
            {
                if (!e.Empty())
                {
                    const auto rehash     = std::hash<T>{}(e.Frost().first);
                    const auto rehash_idx = rehash % m_Size;

                    m_Table[rehash_idx] = std::move(e);
                }
            }
        }
		
		if (!m_Table[hash_idx].Empty())
		{
            auto ref = m_Table[hash_idx].end();
            for (auto it = m_Table[hash_idx].begin(); it != m_Table[hash_idx].end(); ++it)
			{
				if ((*it).first == key)
				{
                    ref = it;
					break;
				}	
			}

			if (ref != m_Table[hash_idx].end())
            {
				(*ref).second = value;
                return true;
            }
		}
        m_Table[hash_idx].PushFront(my::Pair<T, U>::New(key, value));
		
		if (m_MaxListSize < m_Table[hash_idx].Size())
			m_MaxListSize = m_Table[hash_idx].Size();

        return false;
    }

    HASHMAP_TEMPLATE_DECL()
    U& HashMap<T, U>::At(const T& key)
    {
        const auto hash     = std::hash<T>{}(key);
        const auto hash_idx = hash % m_Size;

        if (hash_idx >= m_Size)
            throw std::invalid_argument("That's right, it's an invalid argument.");
        else
        {
            if (m_Table.Empty())
                throw std::invalid_argument("It's empty my guy.");

            for (auto& e : m_Table[hash_idx])
            {
                if (e.first == key)
                    return e.second;
            }

            throw std::invalid_argument("It does not exist.");
        }
    }

    HASHMAP_TEMPLATE_DECL()
    bool HashMap<T, U>::Erase(const T& key)
    {
        const auto hash = std::hash<T>{}(key);
        const auto hash_idx = hash % m_Size;

        if (hash_idx >= m_Size)
            throw std::invalid_argument("That's right, it's an invalid argument.");
        else
        {
            if (m_Table.Empty())
                throw std::invalid_argument("It's empty my guy.");

            for (auto it = m_Table[hash_idx].begin(); it != m_Table[hash_idx].end(); ++it)
            {
                if ((*it).first == key)
                {
                    m_Table[hash_idx].Erase(it);
                    return true;
                }
            }
        }

        return false;
    }

    HASHMAP_TEMPLATE_DECL()
    bool HashMap<T, U>::ContainsKey(const T& key) const noexcept
    {
        const auto hash = std::hash<T>{}(key);
        const auto hash_idx = hash % m_Size;
        for (const auto& e : m_Table[hash_idx])
        {
            if (e.first == key)
                return true;
        }
        return false;
    }

    HASHMAP_TEMPLATE_DECL()
    bool HashMap<T, U>::ContainsValue(const U& value) const noexcept
    {
        for (const auto& l : m_Table)
        {
            for (const auto& e : l)
            {
                if (e.second == value)
                    return true;
            }
        }
        return false;
    }

    HASHMAP_TEMPLATE_DECL()
    usize HashMap<T, U>::Size() const noexcept
    {
        usize sum = 0;
        for (const auto& l : m_Table)
            sum += l.Size();
        return sum;
    }

    HASHMAP_TEMPLATE_DECL()
    bool HashMap<T, U>::Empty() const noexcept
    {
        for (const auto& l : m_Table)
            for (const auto& e : l)
                return false;
        return true;
    }

    HASHMAP_TEMPLATE_DECL()
    void HashMap<T, U>::Clear() noexcept
    {
        for (const auto& l : m_Table)
            l.Clear();
    }

    HASHMAP_TEMPLATE_DECL()
    my::Vec<U> HashMap<T, U>::Values() const noexcept
    {
        my::Vec<U> values{};
        for (const auto& l : m_Table)
            for (const auto& e : l)
                values.Push(e.second);
        return values;
    }

    HASHMAP_TEMPLATE_DECL()
    std::unordered_set<T> HashMap<T, U>::KeySet() const noexcept
    {
        std::unordered_set<T> set{};
        for (const auto& l : m_Table)
            for (const auto& e : l)
                set.insert(e.first);
        return set;
    }

    HASHMAP_TEMPLATE_DECL()
    std::unordered_set<U> HashMap<T, U>::ValueSet() const noexcept
    {
        std::unordered_set<U> set{};
        for (const auto& l : m_Table)
            for (const auto& e : l)
                set.insert(e.second);
        return set;
    }
} // namespace my

#undef HASHMAP_TEMPLATE_DECL

#endif // MY_HASH_MAP_IMPL_HPP
