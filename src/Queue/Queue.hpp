#ifndef QUEUE_IMPL_H
#define QUEUE_IMPL_H

namespace stl {
    template <typename T>
    Queue<T>::Queue() noexcept = default;

    template <typename T>
    Queue<T>::Queue(const std::initializer_list<T> list) noexcept
    {
        Realloc(list.size());
        std::copy(list.begin(), list.end(), m_Buffer);
    }

    template <typename T>
    Queue<T>::Queue(const Queue<T>& other) noexcept
    {
        if (other.m_Buffer)
        {
            m_Size         = other.m_Size;
            m_Capacity     = other.m_Capacity;
            m_IndexPointer = other.m_IndexPointer;
            m_Buffer       = new T[m_Capacity];
            std::memcpy(m_Buffer, other.m_Buffer, m_Size * sizeof(T));
        }
    }

    template <typename T>
    Queue<T>::Queue(Queue<T>&& other) noexcept
    {
        if (other.m_Buffer)
        {
            m_Buffer       = other.m_Buffer;
            m_Size         = other.m_Size;
            m_Capacity     = other.m_Capacity;
            m_IndexPointer = other.m_IndexPointer;

            other.m_Buffer       = nullptr;
            other.m_Size         = 0;
            other.m_Capacity     = 0;
            other.m_IndexPointer = 0;
        }
    }

    template <typename T>
    Queue<T>::~Queue() noexcept
    {
        Drop();
    }

    template <typename T>
    void Queue<T>::Realloc(const usize newSize)
    {
        if (newSize == m_Size)
            return;

        if (m_Buffer)
        {
            if (m_IndexPointer >= m_Capacity / 2)
            {
                for (usize j = 0, i = m_IndexPointer; i < m_Size - m_IndexPointer; ++i)
                    m_Buffer[j++] = std::move(m_Buffer[i]);
                m_IndexPointer = 0;
                m_Size         = newSize;
            }
            else
            {
                // TODO: Consider shrinkage perhaps?
                const auto* temp      = m_Buffer;
                const auto  prev_size = m_Size;

                m_Size     = newSize;
                m_Capacity = m_Size * 2;
                m_Buffer   = new T[m_Capacity];
                std::memcpy(m_Buffer, temp, prev_size * sizeof(T));

                delete[] temp;
            }
        }
        else
        {
            m_Size     = newSize;
            m_Capacity = m_Size * 2;
            m_Buffer   = new T[m_Capacity];
        }
    }

    template <typename T>
    constexpr void Queue<T>::Drop() noexcept
    {
        delete[] m_Buffer;
        m_Buffer   = nullptr;
        m_Size     = 0;
        m_Capacity = 0;
    }

    template <typename T>
    void Queue<T>::Push(const T& e)
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
    T Queue<T>::Pop()
    {
        if (Size() <= 0)
            throw std::out_of_range("Tried calling Pop() on an empty Queue.");
        return std::move(m_Buffer[m_IndexPointer++]);
    }

    template <typename T>
    T& Queue<T>::Front()
    {
        if (Size() <= 0)
            throw std::out_of_range("Tried calling Front() on an empty Queue.");
        return m_Buffer[m_IndexPointer];
    }

    template <typename T>
    T& Queue<T>::Back()
    {
        if (Size() <= 0)
            throw std::out_of_range("Tried calling Front() on an empty Queue.");
        return m_Buffer[m_Size - 1];
    }

    template <typename T>
    void Queue<T>::Swap(Queue<T>& other) noexcept
    {
        std::swap(m_Buffer, other.m_Buffer);
        std::swap(m_Size, other.m_Size);
        std::swap(m_Capacity, other.m_Capacity);
        std::swap(m_IndexPointer, other.m_IndexPointer);
    }

    template <typename T>
    inline Queue<T>& Queue<T>::operator=(const Queue<T>& other)
    {
        if (&other == this)
            return *this;

        if (other.m_Buffer)
        {
            if (m_Buffer)
                Drop();

            m_Size         = other.m_Size;
            m_Capacity     = other.m_Capacity;
            m_IndexPointer = other.m_IndexPointer;
            m_Buffer       = new T[m_Capacity];
            std::memcpy(m_Buffer, other.m_Buffer, m_Size * sizeof(T));
        }
        return *this;
    }

    template <typename T>
    inline Queue<T>& Queue<T>::operator=(Queue<T>&& other) noexcept
    {
        if (&other == this)
            return *this;

        if (other.m_Buffer)
        {
            m_Buffer       = other.m_Buffer;
            m_Size         = other.m_Size;
            m_Capacity     = other.m_Capacity;
            m_IndexPointer = other.m_IndexPointer;

            other.m_Buffer       = nullptr;
            other.m_Size         = 0;
            other.m_Capacity     = 0;
            other.m_IndexPointer = 0;
        }
        return *this;
    }
} // namespace stl

#endif // QUEUE_IMPL_H
