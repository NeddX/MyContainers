#ifndef STACK_H
#define STACK_H

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <limits>

#include <CommonDef.h>

namespace stl {
    template <typename T>
    class Stack
    {
    private:
        T*    m_Buffer   = nullptr;
        usize m_Size     = 0;
        usize m_Capacity = 0;

    public:
        class ConstIterator;
        class Iterator
        {
            friend class ConstIterator;

            using iterator_category = std::forward_iterator_tag;
            using difference_type   = ptrdiff;
            using value_type        = T;
            using pointer           = value_type*;
            using reference         = value_type&;

        private:
            pointer m_Ptr;

        public:
            Iterator(pointer ptr) noexcept : m_Ptr(ptr) {}

        public:
            constexpr reference operator*() const noexcept { return *m_Ptr; }
            constexpr pointer   operator->() const noexcept { return m_Ptr; };
            inline Iterator&    operator++() noexcept
            {
                ++m_Ptr;
                return *this;
            }
            inline Iterator operator++(const i32) noexcept
            {
                auto t = *this;
                ++(*this);
                return t;
            }
            constexpr ptrdiff operator-(const Iterator& other) const noexcept { return m_Ptr - other.m_Ptr; }
            inline Iterator   operator+(const uintptr disp) const noexcept
            {
                auto temp = *this;
                temp.m_Ptr += disp;
                return temp;
            };
            inline Iterator operator-(const uintptr disp) const noexcept
            {
                auto temp = *this;
                temp.m_Ptr -= disp;
                return temp;
            }

        public:
            friend bool operator==(const Iterator& lhv, const Iterator& rhv) noexcept { return lhv.m_Ptr == rhv.m_Ptr; }
            friend bool operator!=(const Iterator& lhv, const Iterator& rhv) noexcept { return !(lhv == rhv); }
        };
        class ConstIterator
        {
            using iterator_category = std::forward_iterator_tag;
            using difference_type   = ptrdiff;
            using value_type        = T;
            using pointer           = const value_type*;
            using reference         = const value_type&;

        private:
            pointer m_Ptr;

        public:
            ConstIterator(pointer ptr) noexcept : m_Ptr(ptr) {}
            ConstIterator(Iterator it) noexcept : m_Ptr(it.m_Ptr) {}

        public:
            constexpr reference   operator*() const noexcept { return *m_Ptr; }
            constexpr pointer     operator->() const noexcept { return m_Ptr; };
            inline ConstIterator& operator++() noexcept
            {
                ++m_Ptr;
                return *this;
            }
            inline ConstIterator operator++(const i32) noexcept
            {
                auto t = *this;
                ++(*this);
                return t;
            }
            constexpr ptrdiff    operator-(const ConstIterator& other) const noexcept { return m_Ptr - other.m_Ptr; }
            inline ConstIterator operator+(const i32 disp) const noexcept
            {
                ConstIterator temp = *this;
                temp.m_Ptr += disp;
                return temp;
            };
            inline ConstIterator operator-(const i32 disp) const noexcept
            {
                ConstIterator temp = *this;
                temp.m_Ptr -= disp;
                return temp;
            }

        public:
            friend bool operator==(const ConstIterator& lhv, const ConstIterator& rhv) noexcept
            {
                return lhv.m_Ptr == rhv.m_Ptr;
            }
            friend bool operator!=(const ConstIterator& lhv, const ConstIterator& rhv) noexcept
            {
                return !(lhv == rhv);
            }
        };

    public:
        Stack();
        Stack(const usize size);
        Stack(const std::initializer_list<T> list);
        Stack(const Stack<T>& other);
        Stack(Stack<T>&& other);
        ~Stack();

    public:
        constexpr usize Size() const noexcept { return m_Size; }
        constexpr usize Capacity() const noexcept { return m_Capacity; }
        constexpr bool  Empty() const noexcept { return m_Size == 0; }
        constexpr T*    Data() const noexcept { return m_Buffer; }
        constexpr usize MaxSize() const noexcept { return std::numeric_limits<usize>::max() / sizeof(T); }

    public:
        inline Iterator      begin() noexcept { return Iterator(m_Buffer); }
        inline Iterator      end() noexcept { return Iterator(m_Buffer + m_Size); }
        inline ConstIterator begin() const noexcept { return ConstIterator(m_Buffer); }
        inline ConstIterator end() const noexcept { return ConstIterator(m_Buffer + m_Size); }

    private:
        void        Realloc(const usize newSize, const bool reserveExtra = true);
        inline void Drop() noexcept;

    public:
        void            Push(const T& e);
        inline T        Pop();
        inline T&       Top();
        inline const T& Top() const;
        constexpr void  Swap(Stack<T>& other);

    public:
        template <typename... TArgs>
        void Emplace(TArgs&&... args);

    public:
        inline Stack<T>& operator=(const std::initializer_list<T> list);
        inline Stack<T>& operator=(const Stack<T>& other);
        inline Stack<T>& operator=(Stack<T>&& other) noexcept;

    public:
        friend std::ostream& operator<<(std::ostream& stream, const Stack<T>& other) noexcept
        {
            stream << "[ ";
            for (usize i = 0; i < other.m_Size; ++i)
            {
                if (i + 1 != other.m_Size)
                    stream << other.m_Buffer[i] << ", ";
                else
                    stream << other.m_Buffer[i];
            }
            stream << " ]";
            return stream;
        }
    };
} // namespace stl

// FIXME: Uncomment
#include "Stack.hpp"
#endif // STACK_H
