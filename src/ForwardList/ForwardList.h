#ifndef FORWARD_LIST_H
#define FORWARD_LIST_H

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <limits>
#include <utility>
#include <vector>

#include <CommonDef.h>

namespace my {
    template <typename T>
    class ForwardList
    {
    private:
        struct Node
        {
            T     obj{};
            Node* next = nullptr;

        public:
            Node() noexcept;
            Node(const T& obj) noexcept;
        };
        class ConstIterator;
        class Iterator
        {
            friend class ConstIterator;

            using iterator_category = std::forward_iterator_tag;
            using difference_type   = usize;
            using value_type        = ForwardList<T>;
            using pointer           = value_type*;
            using reference         = T&;

        private:
            pointer m_Ptr;
            usize   m_Index;

        public:
            Iterator(pointer ptr = nullptr, const usize index = 0) noexcept : m_Ptr(ptr), m_Index(index) {}

        public:
            constexpr reference operator*() const noexcept { return m_Ptr->operator[](m_Index); }
            constexpr pointer   operator->() const noexcept { return m_Ptr; };
            inline Iterator&    operator++() noexcept
            {
                m_Index++;
                return *this;
            }
            Iterator operator++(const i32) noexcept
            {
                auto t = *this;
                ++(*this);
                return t;
            }
            constexpr difference_type operator-(const Iterator& other) const noexcept
            {
                return m_Index - other.m_Index;
            }
            inline Iterator operator+(const uintptr disp) const noexcept
            {
                Iterator temp = *this;
                temp.m_Index += disp;
                return temp;
            };
            inline Iterator operator-(const uintptr disp) const noexcept
            {
                Iterator temp = *this;
                temp.m_Index -= disp;
                return temp;
            }

        public:
            friend bool operator==(const Iterator& lhv, const Iterator& rhv) noexcept
            {
                return lhv.m_Ptr == rhv.m_Ptr && lhv.m_Index == rhv.m_Index;
            }
            friend bool operator!=(const Iterator& lhv, const Iterator& rhv) noexcept { return !(lhv == rhv); }
        };
        class ConstIterator
        {
            using iterator_category = std::forward_iterator_tag;
            using difference_type   = usize;
            using value_type        = ForwardList<T>;
            using pointer           = const value_type*;
            using reference         = const T&;

        private:
            pointer m_Ptr;
            usize   m_Index;

        public:
            ConstIterator(const Iterator it) noexcept : m_Ptr(it.m_Ptr), m_Index(it.m_Index) {}
            ConstIterator(pointer ptr = nullptr, const usize index = 0) noexcept : m_Ptr(ptr), m_Index(index) {}

        public:
            constexpr reference operator*() const noexcept
            {
                return (reference)((value_type*)m_Ptr)->operator[](m_Index);
            }
            constexpr pointer     operator->() const noexcept { return m_Ptr; };
            inline ConstIterator& operator++() noexcept
            {
                m_Index++;
                return *this;
            }
            ConstIterator operator++(const i32) noexcept
            {
                auto t = *this;
                ++(*this);
                return t;
            }
            constexpr difference_type operator-(const ConstIterator& other) const noexcept
            {
                return m_Index - other.m_Index;
            }
            inline ConstIterator operator+(const uintptr disp) const noexcept
            {
                ConstIterator temp = *this;
                temp.m_Index += disp;
                return temp;
            };
            inline ConstIterator operator-(const uintptr disp) const noexcept
            {
                ConstIterator temp = *this;
                temp.m_Index -= disp;
                return temp;
            }

        public:
            friend bool operator==(const ConstIterator& lhv, const ConstIterator& rhv) noexcept
            {
                return lhv.m_Ptr == rhv.m_Ptr && lhv.m_Index == rhv.m_Index;
            }
            friend bool operator!=(const ConstIterator& lhv, const ConstIterator& rhv) noexcept
            {
                return !(lhv == rhv);
            }
        };

    private:
        Node* m_Head   = nullptr;
        usize m_Length = 0;

    public:
        ForwardList();
        ForwardList(const std::initializer_list<T> list);
        ForwardList(const ForwardList<T>& other);
        ForwardList(ForwardList<T>&& other) noexcept;
        ~ForwardList();

    private:
        inline void                    Drop() noexcept;
        inline std::pair<Node*, Node*> GetPreviousAndLast() noexcept;
        inline Node*                   GetNodeAt(const usize index);

    public:
        constexpr bool       Empty() const noexcept { return m_Length == 0; }
        constexpr usize      Size() const noexcept { return m_Length; }
        constexpr usize      MaxSize() const noexcept { return std::numeric_limits<usize>::max() / sizeof(Node); }
        inline Iterator      begin() noexcept { return Iterator(this, 0); }
        inline Iterator      end() noexcept { return Iterator(this, m_Length); }
        inline ConstIterator cbegin() const noexcept { return ConstIterator(this, 0); }
        inline ConstIterator cend() const noexcept { return ConstIterator(this, m_Length); }

    public:
        inline void     Push(const T& e);
        inline void     PushFront(const T& e);
        inline T        Pop();
        inline T        PopFront();
        inline T&       Frost();
        inline const T& Frost() const;
        inline void     Clear();
        inline void     Resize(const usize newSize);
        constexpr void  Swap(ForwardList<T>& other);
        void            Erase(const ConstIterator pos);
        void            Erase(const ConstIterator first, const ConstIterator last);
        void            Insert(const ConstIterator pos, const T& e);
        void            Insert(const ConstIterator pos, const ConstIterator first, const ConstIterator last);
        void            Reverse();
        void            Sort();

    public:
        inline T&              operator[](const usize index) noexcept;
        inline ForwardList<T>& operator=(const ForwardList<T>& other);
        inline ForwardList<T>& operator=(ForwardList<T>&& other) noexcept;

    public:
        friend std::ostream& operator<<(std::ostream& stream, const ForwardList<T>& other) noexcept
        {
            stream << "[ ";
            for (auto* c = other.m_Head; c; c = c->next)
            {
                if (c->next)
                    stream << c->obj << ", ";
                else
                    stream << c->obj;
            }
            stream << " ]";
            return stream;
        }
    };
} // namespace my

#include "ForwardList.hpp"
#endif // FORWARD_LIST_H
