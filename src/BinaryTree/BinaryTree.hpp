#ifndef MY_BINARY_TREE_IMPL_H
#define MY_BINARY_TREE_IMPL_H

#include <functional>
#include <queue>
#include <type_traits>

namespace my {
    template <typename T>
    BinaryTree<T>::Node::Node() = default;

    template <typename T>
    BinaryTree<T>::Node::Node(const T& val) : val(val)
    {
    }

    template <typename T>
    BinaryTree<T>::BinaryTree() = default;

    template <typename T>
    BinaryTree<T>::BinaryTree(const std::initializer_list<T> list)
    {
        for (const auto& ref : list)
            Insert(ref);
    }

    template <typename T>
    BinaryTree<T>::BinaryTree(const BinaryTree<T>& other)
    {
        if (other.m_Root)
        {
            // Recursive helper lambda that takes a pointer to the source tree and a pointer reference
            // to the new tree and simply traverses our source tree and creates a new node
            // for our destination tree and copies the current value from the source tree
            // to the destination tree.
            std::function<void(Node*, Node*&)> recv = [&](Node* ptr, Node*& dest)
            {
                // Our current dest pointer is the node to be created so create it and pass
                // our source tree's current value.
                dest = new Node(ptr->val);

                // Now continue traversing through the source tree.
                if (ptr->left)
                    recv(ptr->left, root->left);
                if (ptr->right)
                    recv(ptr->right, root->right);
            };
            recv(other.m_Root, m_Root);
            m_Count = other.m_Count;
        }
    }

    template <typename T>
    BinaryTree<T>::BinaryTree(BinaryTree<T>&& other) noexcept
    {
        if (other.m_Root)
        {
            m_Root        = other.m_Root;
            m_Count       = other.m_Count;
            other.m_Root  = nullptr;
            other.m_Count = 0;
        }
    }

    template <typename T>
    BinaryTree<T>::~BinaryTree()
    {
        Drop(true);
    }

    template <typename T>
    BinaryTree<T>& BinaryTree<T>::operator=(const BinaryTree<T>& other) const
    {
        // So we don't try to assign ourselves to ourselves? Yeah...
        if (this == &other)
            return *this;
        
        // In case we have an active tree.
        if (m_Root)
            Drop();

        // Same as the copy constructor.
        if (other.m_Root)
        {
            // Recursive helper lambda that captures itself by reference intended for 
            // recursively copying from root to ptr.
            std::function<void(Node*, Node*&)> recv = [&recv](Node* ptr, Node*& root)
            {
                // Our 'root' or destination tree is a new node that holds our 
                // source tree's value.
                root = new Node(ptr->val);

                if (ptr->left)
                    recv(ptr->left, root->left);
                if (ptr->right)
                    recv(ptr->right, root->right);
            };
            recv(other.m_Root, m_Root);
            m_Count = other.m_Count;
        }

        return *this;
    }

    template <typename T>
    BinaryTree<T>& BinaryTree<T>::operator=(BinaryTree<T>&& other) noexcept
    {
        // So we don't try to move ourself.
        if (this == &other)
            return *this;

        // Dispose our current tree if there's one.
        if (m_Root)
            Drop();

        // If other is valid.
        if (other.m_Root)
        {
            m_Root        = other.m_Root;
            m_Count       = other.m_Count;
            other.m_Root  = nullptr;
            other.m_Count = 0;
        }

        return *this;
    }

    template <typename T>
    inline void BinaryTree<T>::Drop(const bool destructing)
    {
        // Helper function for properly disposing the tree.

        // Recursive helper lambda that simply goes through all the nodes
        // and calls delete on them.
        std::function<void(Node*)> recv = [&](Node* ptr)
        {
            if (ptr->left)
                recv(ptr->left);
            if (ptr->right)
                recv(ptr->right);

            delete ptr;
        };

        recv(m_Root);
        m_Root = nullptr;
    }

    template <typename T>
    void BinaryTree<T>::Insert(const T& val)
    {
        // If T is a number, check if it is negative, if so, ignore and move on.
        if (std::is_arithmetic<T>::value)
            if (val < 0)
                return;

        // If its the first node.
        if (!m_Root)
            m_Root = new Node(val);
        else
        {
            // If its not the first node then try to insert.
            auto* current = m_Root;

            // If our current node pointer is valid and we do not want to insert an already exsiting value.
            while (current && current->val != val)
            {
                // If the val is bigger than the current node's val.
                if (val < current->val)
                {
                    // If we do not have a left node then this is it, create the node and store our new val.
                    if (!current->left)
                    {
                        current->left = new Node(val);
                        ++m_Count;
                        break;
                    }
                    else
                        // Else go left.
                        current = current->left;
                }
                else
                {
                    // If we do not have a right node then this is it, create the node and store out new val.
                    if (!current->right)
                    {
                        current->right = new Node(val);
                        ++m_Count;
                        break;
                    }
                    else
                        // Else go right.
                        current = current->right;
                }
            }
        }
    }

    template <typename T>
    void BinaryTree<T>::Remove(const T& val)
    {
        bool  found   = false;   // Boolean flag for determining if our value was found or not.
        auto* current = m_Root;  // Current node pointer.
        auto* prev    = current; // Previous node pointer.

        // Try and find the value.
        while (current)
        {
            // Store current pointer as the previous.
            prev = current;

            // Did we find it?
            if (current->val == val)
            {
                // Found it, yay.
                found = true;
                break;
            }
            // Nope, go left if the value is smaller.
            else if (current->val > val)
                current = current->left;
            else
                current = current->right; // Else go right.
        }

        // In case the value that we want to remove doesn't actually exist in our tree.
        if (!found)
            return;

        // If it is the head that we are trying to remove.
        if (current == m_Root)
        {
            // Grab the found node's left pointer and then go all the way to the left
            // until there's no left node left (pun not intended).
            auto* current2 = current->right;
            while (current2->left)
                current2 = current2->left;

            // Assign the root (m_Root) to the leftmost node of the right node (current2) of the to be deleted node
            // (current).
            m_Root = current2;

            // Assign the new root's left node to be the to be deleted node's right node.
            m_Root->right = current->right;

            // And finally delete it.
            delete current;
            return;
        }

        // If it is not the root and our node doesn't have a right node.
        if (!current->right)
        {
            // See if it has a left pointer.
            if (current->left)
            {
                if (prev->left == current)
                    prev->left = current->left;
                else
                    prev->right = current->right;

                delete current;
                return;
            }
            else // If it doesn't then it's a leaf, easy peasy.
            {
                // If to be deleted node is previous' left node,
                // then assign it to nullptr.
                if (prev->left == current)
                    prev->left = nullptr;
                else // Else it's the right.
                    prev->right = nullptr;

                // Finally, delete it.
                delete current;
                return;
            }
        }
        else
        {
            // If we do have a right node then assign it to current2 and then
            // go left as much as we can until we are at the leftmost node.
            auto* current2 = current->right;
            while (current2->left)
                current2 = current2->left;

            // If to be deleted node is previous' left node,
            // then assign previous' left to point to our leftmost node (current2).
            if (prev->left == current)
                prev->left = current2;
            else // Else it's the right.
                prev->right = current2;

            // Assign our leftmost node's left to point to our to be deleted node's right.
            current2->left = current->right;

            // Finally, delete it.
            delete current;
            return;
        }
    }

    template <typename T>
    bool BinaryTree<T>::Search(const T& val) const noexcept
    {
        // Our current node pointer.
        auto* current = m_Root;

        // If our current node pointer is valid, meaning there's still nodes left in our tree.
        while (current)
        {
            // If our current node's value is equal to the value we want then ladies and gentlemen, we've got him.
            if (current->val == val)
                return true;
            else if (current->val > val) // Else if our current value is bigger than the value to be searched then go
                                         // left.
                current = current->left;
            else
                current = current->right; // Else it's the opposite so go left.
        }

        // If we reach here then that means we traversed through the tree and were unsuccessful so return false.
        return false;
    }

    template <typename T>
    std::vector<T> BinaryTree<T>::InOrder() const noexcept
    {
        // Our vector which will hold our traversed elements.
        std::vector<T> vec;

        // Recurisve helper lambda which will traverse through the tree that captures itself and vec
        // and takes a Node pointer.
        std::function<void(Node*)> recv = [&vec, &recv](Node* ptr)
        {
            // If there's a right then traverse to the leftmost node.
            if (ptr->left)
                recv(ptr->left);

            // Push our value, at this point we are the left most node
            // which will automatically give us our desired behaviour meaning
            // our left most node will always be pushed first.
            vec.push_back(ptr->val);

            // If there's a right node then traverse to the rightmost node.
            if (ptr->right)
                recv(ptr->right);
        };

        // Optimization
        vec.reserve(m_Count);

        recv(m_Root);
        return vec;
    }

    template <typename T>
    std::vector<T> BinaryTree<T>::PostOrder() const noexcept
    {
        // Our vector which will hold our traversed elements.
        std::vector<T> vec;

        // Recurisve helper lambda which will traverse through the tree that captures itself and vec
        // and takes a Node pointer.
        std::function<void(Node*)> recv = [&vec, &recv](Node* ptr)
        {
            // If there's a right then traverse to the leftmost node.
            if (ptr->left)
                recv(ptr->left);

            // Push our value, at this point we are the left most node
            // which will automatically give us our desired behaviour meaning
            // our left most node will always be pushed first.
            if (ptr->right)
                recv(ptr->right);

            // Push our value, at this point we are the left most node
            // which will automatically give us our desired behaviour.
            vec.push_back(ptr->val);
        };

        // Optimization
        vec.reserve(m_Count);

        recv(m_Root);
        return vec;
    }

    template <typename T>
    std::vector<T> BinaryTree<T>::LevelOrder() const noexcept
    {
        std::vector<T>    vec;
        std::queue<Node*> queue;
        queue.push(m_Root);
        while (!queue.empty())
        {
            auto* current = queue.front();
            if (current->left)
                queue.push(current->left);
            if (current->right)
                queue.push(current->right);
            vec.emplace_back(current->val);
            queue.pop();
        }
        return vec;
    }

    template <typename T>
    std::vector<T> BinaryTree<T>::PreOrder() const noexcept
    {
        std::vector<T>             vec;
        std::function<void(Node*)> recv = [&recv](Node* ptr)
        {
            vec.push_back(ptr->val);

            if (ptr->left)
                recv(ptr->left);

            if (ptr->right)
                recv(ptr->right);
        };

        vec.reserve(m_Count);
        recv(m_Root);
        return vec;
    }

    template <typename T>
    T BinaryTree<T>::FindMax() const noexcept
    {
        auto* current = m_Root;
        while (current->right)
            current = current->right;
        return current->val;
    }

    template <typename T>
    T BinaryTree<T>::FindMin() const noexcept
    {
        auto* current = m_Root;
        while (current->left)
            current = current->left;
        return current->val;
    }

    template <typename T>
    usize BinaryTree<T>::Height() const noexcept
    {
        // Our current height holder.
        usize                      height = 0;

        // Maximum value height reached which is our actual height really.
        usize                      max    = 0;

        // Recursive lambda that will traverse through each node, increment 
        std::function<void(Node*)> recv   = [&recv](Node* ptr)
        {
            ++height;
            if (ptr->left)
                recv(ptr->left);
            if (ptr->right)
                recv(ptr->right);

            if (max < height)
                max = height;
            --height;
        };
        recv(m_Root);
        return max;
    }

    template <typename T>
    std::vector<T> BinaryTree<T>::Serialize() const noexcept
    {
        // Serialize is InOrder in disguise.
        return InOrder();
    }

    template <typename T>
    const T& BinaryTree<T>::Successor(const T& val) const
    {
        // Our current node pointer.
        auto* current  = m_Root;

        // Our ancestor which will hold the previous least biggest value.
        Node* ancestor = nullptr;
        while (current)
        {
            // If the current value is the value provided.
            if (current->val == val)
            {
                // If there's no ancestor and there's no right pointer than
                // there's no successor so return the provided value back.
                if (!ancestor && !current->right)
                    return current->val;
                else if (!current->right) // Else if there's no left node then return the ancestor.
                    return ancestor->val;
                else 
                    return current->right->val; // Else just return the current's right node's value.
            }
            else if (current->val > val) // If the current value is bigger than the provided value then our ancestor becomes the current node.

                ancestor = current;

            // If our current value is bigger than the provided value then assign current to point to it's left node.
            if (current->val > val)
                current = current->left;
            else // Else make current point to it's right node.
                current = current->right;
        }

        // If we reach here then the value doesn't exist.
        throw std::invalid_argument("Value does not exist.");
    }

    template <typename T>
    const T& BinaryTree<T>::Predecessor(const T& val) const
    {
        // Our current node pointer.
        auto* current  = m_Root;

        // Our ancestor which will hold the previous least smallest value.
        Node* ancestor = nullptr;
        while (current)
        {
            // If the current value is the value provided.
            if (current->val == val)
            {
                // If there's no ancestor and there's no left pointer than
                // there's no predecessor so return the provided value back.
                if (!ancestor && !current->left)
                    return current->val;
                else if (!current->left) // Else if there's no left node then return the ancestor.
                    return ancestor->val;
                else
                    return current->left->val; // Else just return the current's left node's value.
            }
            else if (current->val < val) // If the current value is lesser than the provided value then our ancestor becomes the current node.
                ancestor = current;

            // If our current value is lesser than the provided value then assign current to point to it's left node.
            if (current->val > val)
                current = current->left;
            else // Else make current point to it's right node instead.
                current = current->right;
        }

        // If we reach here then the value doesn't exist.
        throw std::invalid_argument("Value does not exist.");
    }

    template <typename T>
    std::vector<T> BinaryTree<T>::RangeQuery(const T& begin, const T& end) const
    {
        // If begin is bigger than end then return an empty vector.
        if (begin > end)
            return {};

        std::vector<T>                   vec;

        // Recursive helper lambda that captures itself as a reference and vec as a reference
        // and grabs all the value that fall in the range of begin and end and pushes them into vec.
        const std::function<void(Node*)> recv = [&recv, &vec](Node* ptr)
        {
            // If our current value is bigger than begin and lesser than end
            // then push it and continue traversing.
            if (ptr->val > begin && ptr->val < end)
            {
                vec.emplace_back(ptr->val);
                if (ptr->left)
                    recv(ptr->left);
                if (ptr->right)
                    recv(ptr->right);
            }
            else // Else if it does not fall in the range.
            {
                // If our val is lesser than begin then go right.
                if (ptr->val < begin)
                {
                    if (ptr->right)
                        recv(ptr->right);
                }
                else if (ptr->val > end) // Else if it is bigger than end then go left.
                {
                    if (ptr->left)
                        recv(ptr->left);
                }
            }
        };
        recv(m_Root);
        return vec;
    }
} // namespace my

#endif // MY_BINARY_TREE_IMPL_H
