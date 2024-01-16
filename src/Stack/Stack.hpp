#ifndef STACK_IMPL_H
#define STACK_IMPL_H

#include "Stack.h"

namespace stl {
    template <typename T>
    Stack<T>::Stack() = default;

    template <typename T>
    Stack<T>::Stack(const usize size) : m_Size(size), m_Capacity(size * 2), m_Buffer(new T[m_Capacity])
    {
    }

    template <typename T>
    Stack<T>::Stack(const std::initializer_list<T> list)
    {
        m_Size     = list.size();
        m_Capacity = m_Size * 2;
        m_Buffer   = new T[m_Capacity];
        if (!m_Buffer)
            throw std::bad_alloc();
        std::copy(list.begin(), list.end(), m_Buffer);
    }

    template <typename T>
    Stack<T>::Stack(const Stack<T>& other)
    {
        if (&other == this)
            return;

        m_Size     = other.m_Size;
        m_Capacity = other.m_Capacity;
        m_Buffer   = new T[m_Capacity];
        if (!m_Buffer)
            throw std::bad_alloc();
        std::memcpy(m_Buffer, other.m_Buffer, m_Size * sizeof(T));
    }

    template <typename T>
    Stack<T>::Stack(Stack<T>&& other)
    {
        if (&other == this)
            return;

        m_Size           = other.m_Size;
        m_Capacity       = other.m_Capacity;
        other.m_Size     = 0;
        other.m_Capacity = 0;
        std::swap(m_Buffer, other.m_Buffer);
    }

    template <typename T>
    Stack<T>::~Stack()
    {
        Drop();
    }

    template <typename T>
    void Stack<T>::Realloc(const usize newSize, const bool reserveExtra)
    {
        if (newSize == m_Size)
            return;

        const usize prev_size = m_Size;
        m_Size                = newSize;

        if (reserveExtra)
            m_Capacity = newSize * 2;
        else
            m_Capacity = newSize;

        if (prev_size > 0)
        {
            T* temp  = m_Buffer;
            m_Buffer = new T[m_Capacity];
            if (!m_Buffer)
                throw std::bad_alloc();
            if (prev_size < m_Size)
                std::memcpy(m_Buffer, temp, prev_size * sizeof(T));
            else
                std::memcpy(m_Buffer, temp, m_Size * sizeof(T));
            delete[] temp;
        }
        else
        {
            if (m_Buffer)
                delete[] m_Buffer;
            m_Buffer = new T[m_Capacity];
            if (!m_Buffer)
                throw std::bad_alloc();
        }
    }

    template <typename T>
    inline void Stack<T>::Drop() noexcept
    {
        delete[] m_Buffer;
        m_Buffer   = nullptr;
        m_Size     = 0;
        m_Capacity = 0;
    }

    template <typename T>
    void Stack<T>::Push(const T& e)
    {
        if (m_Size >= m_Capacity)
        {
            Realloc(m_Size + 1);
            m_Buffer[m_Size - 1] = e;
        }
        else
            m_Buffer[m_Size++] = e;
    }

    template <typename T>
    inline T Stack<T>::Pop()
    {
        if (m_Size > 0)
            return std::move(m_Buffer[m_Size--]);
        else
            throw std::out_of_range("Tried calling Pop() on an empty vector.");
    }

    template <typename T>
    inline T& Stack<T>::Top()
    {
        if (m_Size > 0)
            return m_Buffer[0];
        else
            throw std::out_of_range("Tried calling Front() on an empty vector.");
    }

    template <typename T>
    inline const T& Stack<T>::Top() const
    {
        if (m_Size > 0)
            return m_Buffer[0];
        else
            throw std::out_of_range("Tried calling Front() on an empty vector.");
    }

    template <typename T>
    constexpr void Stack<T>::Swap(Stack<T>& other)
    {
        std::swap(m_Size, other.m_Size);
        std::swap(m_Capacity, other.m_Capacity);
        std::swap(m_Buffer, other.m_Buffer);
    }

    template <typename T>
    template <typename... TArgs>
    void Stack<T>::Emplace(TArgs&&... args)
    {
        const T* temp = m_Buffer;

        ++m_Size;
        m_Capacity = m_Size * 2;
        m_Buffer   = new T[m_Capacity];

        std::copy(temp, temp, m_Buffer);
        m_Buffer[0] = T(std::forward<TArgs>(args)...);
        std::copy(temp, temp + m_Size - 1, m_Buffer + 1);

        delete[] temp;
    }

    template <typename T>
    inline Stack<T>& Stack<T>::operator=(const std::initializer_list<T> list)
    {
        Realloc(list.size());
        std::copy(list.begin(), list.end(), begin());
        return *this;
    }

    template <typename T>
    inline Stack<T>& Stack<T>::operator=(const Stack<T>& other)
    {
        if (&other == this)
            return *this;

        Realloc(other.m_Size);
        std::memcpy(m_Buffer, other.m_Buffer, m_Size * sizeof(T));
        return *this;
    }

    template <typename T>
    inline Stack<T>& Stack<T>::operator=(Stack<T>&& other) noexcept
    {
        if (&other == this)
            return *this;

        Drop();
        m_Size           = other.m_Size;
        m_Capacity       = other.m_Capacity;
        other.m_Size     = 0;
        other.m_Capacity = 0;
        std::swap(m_Buffer, other.m_Buffer);
        return *this;
    }
} // namespace stl

#endif // STACK_IMPL_H
