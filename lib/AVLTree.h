#ifndef _AVL_TREE_H_
#define _AVL_TREE_H_

#include <memory>
#include "ourUtilityFunctions.h"
#include <exception>

// comment out for no testing
#define TESTING

#ifdef TESTING
// includes for testing
#include <iostream>
#endif // TESTING

// for comparing
enum class Comparison
{
    less = 0,
    equal = 1,
    greater = 2
};

template <typename T>
inline Comparison AVLTree_CompareUsingOperators(const T &left, const T &right)
{
    if (left < right)
        return Comparison::less;
    else if (left > right)
        return Comparison::greater;
    else
        return Comparison::equal;
}

template <typename DATA_TYPE, Comparison (*compFunction)(const DATA_TYPE &, const DATA_TYPE &) = AVLTree_CompareUsingOperators<DATA_TYPE>>
class AVLTree
{
public:
    // error classes

    // if an element do not exists
    class NoSuchElementException : public std::exception
    {
    public:
        const char *what() const noexcept override { return "There is no such element"; }
    };
    class ElementAlreadyExistsException : public std::exception
    {
    public:
        const char *what() const noexcept override { return "Element already exists"; }
    };

    // methods

    AVLTree();
    ~AVLTree() = default;
    AVLTree(const AVLTree &src);

    AVLTree &operator=(const AVLTree &src);
    void clear();
    bool isEmpty() const;
    int height() const;
    int getSize() const;
    /**
     * @return true if element inserted successfully
     * @throw ElementAlreadyExistsException
     * @brief
     * @tparam
     * Time_Complexity:
     * @tparam
     * Space_Complexity:
     */
    void insert(const DATA_TYPE &data);
    /**
     * @return none
     * @throw NoSuchElementException
     * @brief
     * @tparam
     * Time_Complexity:
     * @tparam
     * Space_Complexity:
     */
    bool remove(const DATA_TYPE &data);
    DATA_TYPE &find(const DATA_TYPE &data);
    const DATA_TYPE &getMax() const;
    const DATA_TYPE &getMin() const;

    template <typename FunctionObject>
    void in_order_traversal(FunctionObject do_something) 
    {
        in_order_traversal_aux_recursive(__root.get(), do_something);
    }
    template <typename FunctionObject>
    void reverse_in_order_traversal(FunctionObject do_something) 
    {
        reverse_in_order_traversal_aux_recursive(__root.get(), do_something);
    }
private:
    struct Node;
    using Node_pointer = Node *;
    using Node_unique_ptr = std::unique_ptr<Node>;

    Node_unique_ptr __root;
    int __size;
    Node_pointer __min_element_ptr;
    Node_pointer __max_element_ptr;

    /* Helper Functions
    bool rotateLeft(Node_unique_ptr &node_ptr);
    bool rotateRight(Node_unique_ptr &node_ptr);
    Node_unique_ptr &getUniquePtr(const Node &node);
    bool swapWithLeftChild(Node_unique_ptr &node_ptr);
    bool swapWithRightChild(Node_unique_ptr &node_ptr);
    void swapData(Node_unique_ptr &first, Node_unique_ptr &second);
    void swap(Node_unique_ptr &first, Node_unique_ptr &second);
    template<FunctionObject do_something>
    bool in_order_traversal_aux() const;
    */

#ifdef TESTING
public:
    void print() const;

private:
    void printNode(const Node_pointer &node_ptr, int depth) const;

#endif // TESTING

    Node_pointer getRoot() {return __root.get();}
    struct Node
    {
        DATA_TYPE __data;
        Node_pointer __parent;
        Node_unique_ptr __left;
        Node_unique_ptr __right;
        int __height;

        Node(const DATA_TYPE &data) : __data(data),  __parent(nullptr), __left(nullptr), __right(nullptr), __height(0) {}

        ~Node() = default;

        Node(const Node &src) : __parent(nullptr), __data(src.getData()), __left(nullptr), __right(nullptr), __height(src.__height)
        {
            if (src.hasLeft())
            {
                setLeft(src.__left->getData());
            }
            if (src.hasRight())
            {
                setRight(src.__right->getData());
            }
        }

        DATA_TYPE &getData()
        {
            return __data;
        }

        Node_pointer getLeftPointer() const
        {
            return __left.get();
        }

        Node_pointer getRightPointer() const
        {
            return __right.get();
        }

        void setLeft(const DATA_TYPE &data)
        {
            __left.reset(new Node(data));
            __left->__parent = this;
            Node_pointer temp = this;
            while (temp != nullptr)
            {
                temp->recalculateHeight();
                temp = temp->__parent;
            }
        }

        void setRight(const DATA_TYPE &data)
        {
            __right.reset(new Node(data));
            __right->__parent = this;
            Node_pointer temp = this;
            while (temp != nullptr)
            {
                temp->recalculateHeight();
                temp = temp->__parent;
            }
        }

        void recalculateHeight()
        {
            __height = 1 + our::max((hasLeft()) ? (__left->__height) : (-1), (hasRight()) ? (__right->__height) : (-1));
        }

        bool isLeaf() const
        {
            return ((__left == nullptr) && (__right == nullptr));
        }

        bool isRoot() const
        {
            return (__parent == nullptr);
        }

        bool hasLeft() const
        {
            return (!(__left == nullptr));
        }

        bool hasRight() const
        {
            return (!(__right == nullptr));
        }

        int balanceFactor() const
        {
            return ((hasLeft()) ? (__left->__height) : (-1)) - ((hasRight()) ? (__right->__height) : (-1));
        }

        bool isAVLBalanced() const
        {
            return our::abs(balanceFactor()) <= 1;
        }
    };

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
       +---------------------------------------------------------------+
       |                                                               |
       |        AVLTree Helper Functions Implementations               |
       |                                                               |
       +---------------------------------------------------------------+
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
    /**
     * @brief takes in a pointer for the node to be rotated to the left
     *
     * @warning check for the root (special case)
                 A                                     B
                / \                                   / \
               /   \                                 /   \
              B     a           ==>                 c     A
             / \                                         / \
            /   \                                       /   \
           c     b                                     b     a
    */
    bool rotateLeft(Node_unique_ptr &node_ptr) // node_ptr is pointing to A
    {
        if (node_ptr.get() == nullptr || node_ptr->isLeaf() || !node_ptr->hasLeft())
        {
            return false;
        }
        // else, we can rotate
        Node_unique_ptr &pointer_to_root = node_ptr;
        Node_unique_ptr &pointer_to_A_left = pointer_to_root->__left;
        Node_unique_ptr &pointer_to_B_right = pointer_to_A_left->__right;

        Node_unique_ptr temp_unique_pointer;
        pointer_to_B_right.swap(temp_unique_pointer);
        pointer_to_B_right.swap(pointer_to_root);
        pointer_to_A_left.swap(temp_unique_pointer);
        pointer_to_root.swap(temp_unique_pointer);

        /**
         * now:
         * pointer_to_root points to B
         * pointer_to_A_left points to b
         * pointer_to_B_right points to A
         */

        pointer_to_root->__parent = pointer_to_B_right->__parent; // B's parent points to A's previous parent
        pointer_to_B_right->__parent = pointer_to_root.get();     // B is the parent of A
        if (pointer_to_A_left.get() != nullptr)
        {
            pointer_to_A_left->__parent = pointer_to_B_right.get(); // A is the parent of b
        }

        pointer_to_B_right->recalculateHeight();
        pointer_to_root->recalculateHeight();

        return true;
    }
    /**
     * @brief takes in a pointer for the node to be rotated to the right
     *
     * @warning check for the root (special case)
                     A                                        B
                    / \                                      / \
                   /   \                                    /   \
                  a     B              ==>                 A     c
                       / \                                / \
                      /   \                              /   \
                     b     c                            a     b
    */
    bool rotateRight(Node_unique_ptr &node_ptr)
    {
        if (node_ptr.get() == nullptr || node_ptr->isLeaf() || !node_ptr->hasRight())
        {
            return false;
        }
        // else, we can rotate
        Node_unique_ptr &pA = node_ptr;
        Node_unique_ptr &pB = pA->__right;
        Node_unique_ptr &pb = pB->__left;

        Node_unique_ptr tempPtr;
        pb.swap(tempPtr);
        pb.swap(pA);
        pB.swap(tempPtr);
        pA.swap(tempPtr);

        /**
         * now:
         * pA points to B
         * pB points to b
         * pb points to A
         */

        pA->__parent = pb->__parent; // B's parent points to A's previous parent
        pb->__parent = pA.get();     // B is the parent of A
        if (pB.get() != nullptr)
        {
            pB->__parent = pb.get();
        } // A is the parent of b

        pb->recalculateHeight();
        pA->recalculateHeight();

        return true;
    }
    Node_unique_ptr &getUniquePtr(const Node &node)
    {
        if (node.isRoot())
            return __root;
        if (node.__parent->getLeftPointer() == &node)
        {
            return node.__parent->__left;
        }
        else
        {
            return node.__parent->__right;
        }
    }
    /**
     * @brief takes in a pointer for the node to be rotated to the left
     *
     * @warning check for the root (special case)
                 A                                     B
                / \                                   / \
               /   \                                 /   \
              B     a           ==>                 A     a
             / \                                   / \
            /   \                                 /   \
           c     b                               c     b
    */
    bool swapWithLeftChild(Node_unique_ptr &node_ptr)
    {
        if ((node_ptr.get() == nullptr) || (node_ptr->isLeaf()) || !(node_ptr->hasLeft()))
        {
            return false;
        }
        Node_unique_ptr &pA = node_ptr;
        Node_unique_ptr &pB = pA->__left;
        Node_unique_ptr &pb = pB->__right;
        Node_unique_ptr &pc = pB->__left;
        Node_unique_ptr &pa = pA->__right;
        Node_unique_ptr tempPtr;

        pa.swap(pb);
        pc.swap(tempPtr);
        pc.swap(pA);
        pB.swap(tempPtr);
        pA.swap(tempPtr);

        /**
         * now:
         * pA points to B
         * pB points to c
         * pb points to a
         * pa points to b
         * pc points to A
         */

        pA->__parent = pc->__parent;
        pc->__parent = pA.get();
        if (pB.get() != nullptr)
        {
            pB->__parent = pc.get();
        }
        if (pb.get() != nullptr)
        {
            pb->__parent = pA.get();
        }
        if (pa.get() != nullptr)
        {
            pa->__parent = pc.get();
        }
        our::swap(pA->__height, pc->__height);

        return true;
    }
    /**
     * @brief takes in a pointer for the node to be rotated to the right
     *
     * @warning check for the root (special case)
                 A                                     B
                / \                                   / \
               /   \                                 /   \
              a     B                 ==>           a     A
                   / \                                   / \
                  /   \                                 /   \
                 c     b                               c     b
    */
    bool swapWithRightChild(Node_unique_ptr &node_ptr)
    {
        if (node_ptr.get() == nullptr || node_ptr->isLeaf() || !node_ptr->hasRight())
        {
            return false;
        }
        Node_unique_ptr &pA = node_ptr;
        Node_unique_ptr &pB = pA->__right;
        Node_unique_ptr &pb = pB->__right;
        Node_unique_ptr &pc = pB->__left;
        Node_unique_ptr &pa = pA->__left;
        Node_unique_ptr tempPtr;

        pa.swap(pc);
        pb.swap(tempPtr);
        pb.swap(pA);
        pB.swap(tempPtr);
        pA.swap(tempPtr);

        /**
         * now:
         * pA points to B
         * pB points to b
         * pb points to A
         * pa points to c
         * pc points to a
         */

        pA->__parent = pb->__parent;
        pb->__parent = pA.get();
        if (pB.get() != nullptr)
        {
            pB->__parent = pb.get();
        }
        if (pc.get() != nullptr)
        {
            pc->__parent = pA.get();
        }
        if (pa.get() != nullptr)
        {
            pa->__parent = pb.get();
        }
        our::swap(pA->__height, pb->__height);

        return true;
    }

    void swapData(Node_unique_ptr &first, Node_unique_ptr &second)
    {
        our::swap(first->getData(), second->getData());
    }
    void swap(Node_unique_ptr &first, Node_unique_ptr &second)
    {
        if (first.get() != second.get()) // don't really need to check for self swap
        {
            if (first->getRightPointer() == second.get())
            {
                swapWithRightChild(first);
            }
            if (first->getLeftPointer() == second.get())
            {
                swapWithLeftChild(first);
            }
            else if (second->getRightPointer() == first.get())
            {
                swapWithRightChild(second);
            }
            else if (second->getLeftPointer() == first.get())
            {
                swapWithLeftChild(second);
            }
            else
            {
                first->__left.swap(second->__left);           // swap left children
                first->__right.swap(second->__right);         // swap right children
                first.swap(second);                           // swap the nodes themselves (what the unique ptrs point to)
                our::swap(first->__parent, second->__parent); // update the parents
                our::swap(first->__height, second->__height);
            }
        }
    }

    template <typename FunctionObject>
    bool in_order_traversal_aux_recursive(Node_pointer root, FunctionObject do_something) const
    {
        if (root == nullptr)
        {
            return false;
        }
        in_order_traversal_aux_recursive(root->getLeftPointer(), do_something);
        do_something(root->getData());
        in_order_traversal_aux_recursive(root->getRightPointer(), do_something);
        return true;
    }

    template <typename FunctionObject>
    bool reverse_in_order_traversal_aux_recursive(Node_pointer root, FunctionObject do_something) const
    {
        if (root == nullptr)
        {
            return false;
        }
        reverse_in_order_traversal_aux_recursive(root->getRightPointer(), do_something);
        do_something(root->getData());
        reverse_in_order_traversal_aux_recursive(root->getLeftPointer(), do_something);
        return true;
    }

    bool insert_aux(const DATA_TYPE &data)
    {
        if (isEmpty())
        {
            __root.reset(new Node(data));
            return true;
        }
        Node_pointer tempNodePtr = __root.get();
        while (!tempNodePtr->isLeaf())
        {
            Comparison result = compFunction(tempNodePtr->getData(), data);
            if (result == Comparison::equal)
            {
                return false;
            }
            else if (result == Comparison::greater)
            {
                tempNodePtr = tempNodePtr->getLeftPointer();
            }
            else if (result == Comparison::less)
            {
                tempNodePtr = tempNodePtr->getRightPointer();
            }
        }
        // check where to go at the leaf
        Comparison result = compFunction(tempNodePtr->getData(), data);
        if (result == Comparison::equal)
        {
            return false;
        }
        else if (result == Comparison::greater)
        {
            tempNodePtr->setLeft(data);
        }
        else if (result == Comparison::less)
        {
            tempNodePtr->setRight(data);
        }
        // now the new data is added

        Node_pointer tempFixHeight = tempNodePtr;
        while (tempFixHeight != nullptr)
        {
            tempFixHeight->recalculateHeight();
            tempFixHeight = tempFixHeight->__parent;
        }
        tempFixHeight = tempNodePtr;

        while ((tempNodePtr != nullptr) && (tempNodePtr->isAVLBalanced()))
        {
            tempNodePtr = tempNodePtr->__parent;
        }

        if (tempNodePtr != nullptr)
        {
            if (tempNodePtr->balanceFactor() >= 2) // left heavy
            {
                if (tempNodePtr->__left->balanceFactor() >= 0) // LL rotation
                {
                    // rotateLeft(tempNodePtr->__left);
                    rotateLeft(getUniquePtr(*(tempNodePtr)));
                }
                else // balanceFactor == -1, LR rotation
                {
                    rotateRight(tempNodePtr->__left);
                    rotateLeft(getUniquePtr(*(tempNodePtr)));
                }
            }
            if (tempNodePtr->balanceFactor() <= -2) // right heavy
            {
                if (tempNodePtr->__right->balanceFactor() <= 0) // RR rotation
                {
                    // rotateRight(tempNodePtr->__right);
                    rotateRight(getUniquePtr(*(tempNodePtr)));
                }
                else // balanceFactor == 1, RL rotation
                {
                    rotateLeft(tempNodePtr->__right);
                    rotateRight(getUniquePtr(*(tempNodePtr)));
                }
            }
        }
        while (tempFixHeight != nullptr)
        {
            tempFixHeight->recalculateHeight();
            tempFixHeight = tempFixHeight->__parent;
        }
        return true;
    }
};

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   +---------------------------------------------------------------+
   |                                                               |
   |                    AVLTree Implementations                    |
   |                                                               |
   +---------------------------------------------------------------+
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

template <typename DATA_TYPE, Comparison (*compFunction)(const DATA_TYPE &, const DATA_TYPE &)>
AVLTree<DATA_TYPE, compFunction>::AVLTree() : __root(nullptr), __size(0),
                                              __min_element_ptr(nullptr),
                                              __max_element_ptr(nullptr)
{
}

template <typename DATA_TYPE, Comparison (*compFunction)(const DATA_TYPE &, const DATA_TYPE &)>
AVLTree<DATA_TYPE, compFunction>::AVLTree(const AVLTree &src) : __root(nullptr), __size(src.__size)
{
    if (!src.isEmpty())
    {
        __root.reset(new Node(*(src.__root)));
    }
}

template <typename DATA_TYPE, Comparison (*compFunction)(const DATA_TYPE &, const DATA_TYPE &)>
AVLTree<DATA_TYPE, compFunction> &AVLTree<DATA_TYPE, compFunction>::operator=(const AVLTree &src)
{
    if (&src != this)
    {
        if (!src.isEmpty())
        {
            __root.reset(new Node(*(src.__root)));
        }
        __size = src.__size;
    }
    return *this;
}

template <typename DATA_TYPE, Comparison (*compFunction)(const DATA_TYPE &, const DATA_TYPE &)>
void AVLTree<DATA_TYPE, compFunction>::clear()
{
    __root.reset();
    __size = 0;
}

template <typename DATA_TYPE, Comparison (*compFunction)(const DATA_TYPE &, const DATA_TYPE &)>
bool AVLTree<DATA_TYPE, compFunction>::isEmpty() const
{
    return __root.get() == nullptr;
}

template <typename DATA_TYPE, Comparison (*compFunction)(const DATA_TYPE &, const DATA_TYPE &)>
int AVLTree<DATA_TYPE, compFunction>::height() const
{
    return __root->__height;
}

template <typename DATA_TYPE, Comparison (*compFunction)(const DATA_TYPE &, const DATA_TYPE &)>
int AVLTree<DATA_TYPE, compFunction>::getSize() const
{
    return __size;
}
template <typename DATA_TYPE, Comparison (*compFunction)(const DATA_TYPE &, const DATA_TYPE &)>
const DATA_TYPE &AVLTree<DATA_TYPE, compFunction>::getMax() const
{
    if (__max_element_ptr == nullptr)
    {
        throw NoSuchElementException();
    }
    return __max_element_ptr->getData();
}

template <typename DATA_TYPE, Comparison (*compFunction)(const DATA_TYPE &, const DATA_TYPE &)>
const DATA_TYPE &AVLTree<DATA_TYPE, compFunction>::getMin() const
{
    if (__min_element_ptr == nullptr)
    {
        throw NoSuchElementException();
    }
    return __min_element_ptr->getData();
}

template <typename DATA_TYPE, Comparison (*compFunction)(const DATA_TYPE &, const DATA_TYPE &)>
void AVLTree<DATA_TYPE, compFunction>::insert(const DATA_TYPE &data)
{
    if (!insert_aux(data)) // not added
    {
        throw ElementAlreadyExistsException();
    }
    __size++;
    __min_element_ptr = __root.get();
    while (__min_element_ptr->getLeftPointer())
    {
        __min_element_ptr = __min_element_ptr->getLeftPointer();
    }
    __max_element_ptr = __root.get();
    while (__max_element_ptr->getRightPointer())
    {
        __max_element_ptr = __max_element_ptr->getRightPointer();
    }
}
template <typename DATA_TYPE, Comparison (*compFunction)(const DATA_TYPE &, const DATA_TYPE &)>
bool AVLTree<DATA_TYPE, compFunction>::remove(const DATA_TYPE &data)
{
    // search for the node(unique pointer)
    Node_pointer tempNodePtr = __root.get();
    while (tempNodePtr != nullptr)
    {
        Comparison result = compFunction(tempNodePtr->getData(), data);
        if (result == Comparison::equal)
        {
            break;
        }
        else if (result == Comparison::greater)
        {
            tempNodePtr = tempNodePtr->getLeftPointer();
        }
        else if (result == Comparison::less)
        {
            tempNodePtr = tempNodePtr->getRightPointer();
        }
    }
    if (tempNodePtr == nullptr)
    {
        return false;
    }
    // now tempNodePtr points to a (valid) node we want to remove
    Node_pointer toDeletePtr = tempNodePtr;
    bool hasLeft = toDeletePtr->hasLeft();
    bool hasRight = toDeletePtr->hasRight();
    if (hasLeft && hasRight)
    {
        toDeletePtr = toDeletePtr->getRightPointer();
        while (toDeletePtr->hasLeft())
        {
            toDeletePtr = toDeletePtr->getLeftPointer();
        }
        our::swap(toDeletePtr->getData(), tempNodePtr->getData());
        // think of better solution
    }
    Node_pointer parentOfToDelete = toDeletePtr->__parent;
    Node_unique_ptr &toDeleteUniquePtr = getUniquePtr(*toDeletePtr);
    hasLeft = toDeletePtr->hasLeft();
    hasRight = toDeletePtr->hasRight();
    if (!hasLeft && !hasRight) // leaf
    {
        toDeleteUniquePtr.reset();
    }
    if (hasLeft && !hasRight)
    {
        rotateLeft(toDeleteUniquePtr); // the node we want to delete is the right son of the node that toDeleteUniquePte points to
        toDeleteUniquePtr->__right.reset();
        parentOfToDelete = toDeletePtr;
    }
    if (!hasLeft && hasRight)
    {
        rotateRight(toDeleteUniquePtr); // the node we want to delete is the left son of the node that toDeleteUniquePte points to
        toDeleteUniquePtr->__left.reset();
        parentOfToDelete = toDeletePtr;
    }

    // now we should update the path in the AVL tree
    Node_pointer temp = parentOfToDelete;
    while (temp != nullptr)
    {
        temp->recalculateHeight();
        if (!temp->isAVLBalanced())
        {
            if (temp->balanceFactor() >= 2) // left heavy
            {
                if (temp->__left->balanceFactor() >= 0) // LL rotation
                {
                    // rotateLeft(temp->__left);
                    rotateLeft(getUniquePtr(*temp));
                }
                else // balanceFactor == -1, LR rotation
                {
                    rotateRight(temp->__left);
                    rotateLeft(getUniquePtr(*temp));
                }
            }
            if (temp->balanceFactor() <= -2) // right heavy
            {
                if (temp->__right->balanceFactor() <= 0) // RR rotation
                {
                    // rotateRight(temp->__right);
                    rotateRight(getUniquePtr(*temp));
                }
                else // balanceFactor == 1, RL rotation
                {
                    rotateLeft(temp->__right);
                    rotateRight(getUniquePtr(*temp));
                }
            }
        }
        temp = temp->__parent;
    }
    __size--;
    if (!isEmpty())
    {
        __min_element_ptr = __root.get();
        while (__min_element_ptr->getLeftPointer())
        {
            __min_element_ptr = __min_element_ptr->getLeftPointer();
        }
        __max_element_ptr = __root.get();
        while (__max_element_ptr->getRightPointer())
        {
            __max_element_ptr = __max_element_ptr->getRightPointer();
        }
    }
    return true;
}

template <typename DATA_TYPE, Comparison (*compFunction)(const DATA_TYPE &, const DATA_TYPE &)>
DATA_TYPE &AVLTree<DATA_TYPE, compFunction>::find(const DATA_TYPE &data)
{
    Node_pointer tempNodePtr = getRoot();
    while (tempNodePtr != nullptr)
    {
        Comparison result = Comparison::equal;
        result = compFunction(tempNodePtr->getData(), data);
        if (result == Comparison::equal)
        {
            return tempNodePtr->getData();
        }
        else if (result == Comparison::greater)
        {
            tempNodePtr = tempNodePtr->getLeftPointer();
        }
        else if (result == Comparison::less)
        {
            tempNodePtr = tempNodePtr->getRightPointer();
        }
    }
    throw NoSuchElementException();
}

#ifdef TESTING
template <typename DATA_TYPE, Comparison (*compFunction)(const DATA_TYPE &, const DATA_TYPE &)>
void AVLTree<DATA_TYPE, compFunction>::print() const
{
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "AVLTree size: " << __size << std::endl;
    printNode(__root.get(), 0);
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
}

template <typename DATA_TYPE, Comparison (*compFunction)(const DATA_TYPE &, const DATA_TYPE &)>
void AVLTree<DATA_TYPE, compFunction>::printNode(const Node_pointer &node_ptr, int level) const
{
    if (node_ptr == nullptr)
    {
        return;
    }

    printNode(node_ptr->getRightPointer(), level + 1);

    // Print indentation based on level
    for (int i = 0; i < level * 4; i++)
    {
        std::cout << " ";
    }

    std::cout << "+--" << node_ptr->getData() << std::endl;

    printNode(node_ptr->getLeftPointer(), level + 1);
}

#endif // TESTING

#endif // _AVL_TREE_H_
