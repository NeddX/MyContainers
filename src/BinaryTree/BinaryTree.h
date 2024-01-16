#ifndef MY_BINARY_TREE_H
#define MY_BINARY_TREE_H

#include <cstdint>
#include <exception>
#include <initializer_list>
#include <vector>

#include <CommonDef.h>

namespace my {
    template <typename T>
    class BinaryTree
    {
    public:
        struct Node
        {
            Node* left  = nullptr;
            Node* right = nullptr;
            T     val{};

        public:
            Node();
            Node(const T& ref);
        };

    private:
        Node* m_Root  = nullptr;
        usize m_Count = 0;

    public:
        BinaryTree();
        BinaryTree(const std::initializer_list<T> list);
        BinaryTree(const BinaryTree<T>& other);
        BinaryTree(BinaryTree<T>&& other) noexcept;
        ~BinaryTree();

    public:
        BinaryTree<T>& operator=(const BinaryTree<T>& other) const;
        BinaryTree<T>& operator=(BinaryTree<T>&& other) noexcept;

    private:
        inline void Drop(const bool destructing = false);

    public:
        constexpr usize Size() const noexcept { return m_Count; }
        constexpr void  Clear() { Drop(); }

    public:
        void           Insert(const T& val);
        void           Remove(const T& val);
        bool           Search(const T& val) const noexcept;
        std::vector<T> InOrder() const noexcept;
        std::vector<T> PostOrder() const noexcept;
        std::vector<T> PreOrder() const noexcept;
        std::vector<T> LevelOrder() const noexcept;
        T              FindMax() const noexcept;
        T              FindMin() const noexcept;
        usize          Height() const noexcept;
        std::vector<T> Serialize() const noexcept;
        const T&       Successor(const T& val) const;
        const T&       Predecessor(const T& val) const;
        std::vector<T> RangeQuery(const T& begin, const T& end) const;
    };
} // namespace my

#include "BinaryTree.hpp"

#endif // MY_BINARY_TREE_H
