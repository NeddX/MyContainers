#ifndef FORWARD_LIST_IMPL_H
#define FORWARD_LIST_IMPL_H

namespace my {
    template <typename T>
    using Node = typename ForwardList<T>::Node;

    template <typename T>
    ForwardList<T>::Node::Node() noexcept = default;

    template <typename T>
    ForwardList<T>::Node::Node(const T& obj) noexcept : obj(obj)
    {
    }

    template <typename T>
    ForwardList<T>::ForwardList(const ForwardList<T>& other)
    {
        if (!other.m_Head)
            return;

        m_Head             = new Node(other.m_Head->obj);
        auto* current      = other.m_Head->next;
        auto* this_current = m_Head;
        while (current)
        {
            this_current->next = new Node(current->obj);
            current            = current->next;
            this_current       = this_current->next;
        }
    }

    template <typename T>
    ForwardList<T>::ForwardList(ForwardList<T>&& other) noexcept
    {
        if (other.m_Head)
        {
            m_Head         = other.m_Head;
            m_Length       = other.m_Length;
            other.m_Head   = nullptr;
            other.m_Length = 0;
        }
    }

    template <typename T>
    ForwardList<T>::ForwardList() = default;

    template <typename T>
    ForwardList<T>::ForwardList(const std::initializer_list<T> list)
    {
        for (const auto& e : list)
            Push(e);
    }

    template <typename T>
    ForwardList<T>::~ForwardList()
    {
        Drop();
    }

    template <typename T>
    inline void ForwardList<T>::Drop() noexcept
    {
        auto* current = m_Head;
        while (current)
        {
            auto* temp = current;
            current    = current->next;
            delete temp;
        }
        m_Head = nullptr;
    }

    template <typename T>
    inline std::pair<Node<T>*, Node<T>*> ForwardList<T>::GetPreviousAndLast() noexcept
    {
        Node* first_last  = m_Head;
        Node* second_last = nullptr;
        Node* current     = m_Head;
        while (current->next)
        {
            second_last = first_last;
            first_last  = first_last->next;
            current     = current->next;
        }
        return { second_last, first_last };
    }

    template <typename T>
    inline ForwardList<T>::Node* ForwardList<T>::GetNodeAt(const usize index)
    {
        if (index < 0 || !m_Head)
            return nullptr;

        auto* current = m_Head;
        auto  count   = 0;

        while (current)
        {
            if (count++ == index)
                return current;
            current = current->next;
        }

        return nullptr;
    }

    template <typename T>
    inline void ForwardList<T>::Push(const T& e)
    {
        if (!m_Head)
            m_Head = new Node(e);
        else
        {
            auto* current = m_Head;
            while (current->next)
                current = current->next;
            current->next = new Node(e);
        }
        ++m_Length;
    }

    template <typename T>
    inline void ForwardList<T>::PushFront(const T& e)
    {
        if (!m_Head)
            m_Head = new Node(e);
        else
        {
            auto* prev   = m_Head;
            m_Head       = new Node(e);
            m_Head->next = prev;
        }
        ++m_Length;
    }

    template <typename T>
    inline T& ForwardList<T>::Frost()
    {
        if (!m_Head)
            throw std::out_of_range("Tried caling Front() on an empty List.");
        return m_Head->obj;
    }

    template <typename T>
    inline const T& ForwardList<T>::Frost() const
    {
        if (!m_Head)
            throw std::out_of_range("Tried caling Front() on an empty List.");
        return m_Head->obj;
    }

    template <typename T>
    inline void ForwardList<T>::Clear()
    {
        Drop();
    }

    template <typename T>
    inline void ForwardList<T>::Resize(const usize newSize)
    {
        if (m_Length == newSize)
            return;

        auto prev_two = GetPreviousAndLast();
        if (newSize > m_Length)
        {
            auto* current = prev_two.second;
            for (usize i = 0; i < newSize - m_Length; ++i)
            {
                current->next = new Node();
                current       = current->next;
            }
            m_Length = newSize;
        }
        else if (newSize < m_Length)
        {
            for (usize i = 0; i < m_Length - newSize; ++i)
            {
                prev_two = GetPreviousAndLast();
                delete prev_two.second;
                prev_two.first->next = nullptr;
            }
            m_Length = newSize;
        }
    }

    template <typename T>
    constexpr void ForwardList<T>::Swap(ForwardList<T>& other)
    {
        std::swap(m_Head, other.m_Head);
        std::swap(m_Length, other.m_Length);
    }

    template <typename T>
    void ForwardList<T>::Erase(const ConstIterator pos)
    {
        if (!Empty())
        {
            auto  index    = (pos == end()) ? pos - begin() - 1 : pos - begin();
            auto* node     = GetNodeAt(index);
            auto* backward = GetNodeAt(index - 1);
            auto* forward  = GetNodeAt(index + 1);

            if (backward)
                if (forward)
                    backward->next = forward;
                else
                    backward->next = nullptr;
            else
                m_Head = forward;
            delete node;
            --m_Length;
        }
        else
            throw std::out_of_range("Tried calling Erase() on an empty vector.");
    }

    template <typename T>
    void ForwardList<T>::Erase(const ConstIterator first, const ConstIterator last)
    {
        if (!Empty())
        {
            const auto start_index = first - begin();
            const auto len         = last - first + 1;
            auto*      forward     = GetNodeAt(last - begin() + 1);
            auto*      backward    = GetNodeAt(first - begin() - 1);

            std::vector<Node*> nodes(len);
            for (usize i = 0; i < len; ++i)
                nodes[i] = GetNodeAt(start_index + i);

            if (backward)
                if (forward)
                    backward->next = forward;
                else
                    backward->next = nullptr;
            else
                m_Head = forward;

            for (const auto& e : nodes)
                delete e;

            m_Length -= len;
        }
        else
            throw std::out_of_range("Tried calling Erase() on an empty list.");
    }

    template <typename T>
    void ForwardList<T>::Insert(const ConstIterator pos, const T& e)
    {
        const auto index    = pos - begin();
        auto*      prev     = GetNodeAt(index - 1);
        auto*      next     = GetNodeAt(index);
        auto*      new_node = new Node(e);
        if (!prev)
            m_Head = new_node;
        else
            prev->next = new_node;
        new_node->next = next;
        ++m_Length;
    }

    template <typename T>
    void ForwardList<T>::Insert(const ConstIterator pos, const ConstIterator first, const ConstIterator last)
    {
        const auto index = pos - begin();
        const auto len   = last - first;
        auto*      prev  = GetNodeAt(index - 1);
        auto*      next  = GetNodeAt(index);

        if (!prev)
        {
            auto* new_head = new Node(*first);
            auto* current  = new_head;
            for (auto it = first + 1; it != last; ++it)
            {
                current->next = new Node(*it);
                current       = current->next;
            }
            m_Head        = new_head;
            current->next = next;
        }
        else
        {
            auto* current = prev;
            for (auto it = first; it != last; ++it)
            {
                current->next = new Node(*it);
                current       = current->next;
            }
            current->next = next;
        }
        m_Length += len;
    }

    template <typename T>
    void ForwardList<T>::Reverse()
    {
        Node* current = m_Head;
        Node* prev    = nullptr;
        Node* next    = current->next;
        while (current)
        {
            next          = current->next;
            current->next = prev;
            prev          = current;
            current       = next;
        }
        m_Head = prev;
    }

    template <typename T>
    void ForwardList<T>::Sort()
    {
        for (auto i = 0; i < m_Length; ++i)
        {
            auto  j    = 0;
            auto* prev = m_Head;
            for (auto* d = m_Head; d && j < m_Length - i; d = d->next, ++j)
            {
                auto* n = d->next;
                if (!n)
                    break;
                if (d->obj > n->obj)
                {
                    d->next = n->next;
                    n->next = d;
                    if (m_Head == d)
                        m_Head = n;
                    else
                        prev->next = n;
                }
                prev = d;
            }
        }
    }

    template <typename T>
    inline T& ForwardList<T>::operator[](const usize index) noexcept
    {
        return GetNodeAt(index)->obj;
    }

    template <typename T>
    inline ForwardList<T>& ForwardList<T>::operator=(const ForwardList<T>& other)
    {
        if (&other == this || !other.m_Head)
            return *this;

        if (m_Head)
            Drop();

        m_Head             = new Node(other.m_Head->obj);
        m_Length           = other.m_Length;
        auto* current      = other.m_Head->next;
        auto* this_current = m_Head;
        while (current)
        {
            this_current->next = new Node(current->obj);
            current            = current->next;
            this_current       = this_current->next;
        }

        return *this;
    }

    template <typename T>
    inline ForwardList<T>& ForwardList<T>::operator=(ForwardList<T>&& other) noexcept
    {
        if (&other == this)
            return *this;

        if (m_Head)
            Drop();

        m_Head         = other.m_Head;
        m_Length       = other.m_Length;
        other.m_Head   = nullptr;
        other.m_Length = 0;
        return *this;
    }

    template <typename T>
    inline T ForwardList<T>::Pop()
    {
        auto prev_two = GetPreviousAndLast();
        auto obj      = std::move(prev_two.first);
        delete prev_two.second;
        prev_two.first->next = nullptr;
        --m_Length;
        return std::move(obj);
    }

    template <typename T>
    inline T ForwardList<T>::PopFront()
    {
        auto  obj = std::move(m_Head->obj);
        auto* t   = m_Head;
        m_Head    = m_Head->next;
        delete t;
        --m_Length;
        return std::move(obj);
    }
} // namespace my

#endif // FORWARD_LIST_IMPL_H
