
#include <memory>
#include "ourUtilityFunctions.h"

template <typename DATA_TYPE>
class AVLTree
{
private:
    struct Node;
    using Node_pointer = Node *;
    using Node_unique_ptr = std::unique_ptr<Node>;

public:
    // error classes

    // if an element do not exists
    class NoSuchElementException : public std::exception
    {
        const char *what() const noexcept override { return "There is no such element"; };
    };
    class ElementAlreadyExistsException : public std::exception
    {
        const char *what() const noexcept override { return "Element already exists"; };
    };

    AVLTree() : __root(nullptr), __size(0) {}
    ~AVLTree() = default;
    AVLTree(const AVLTree &src) : __root(nullptr), __size(src.__size)
    {
        if (!src.isEmpty())
        {
            __root.reset(new Node(*(src.__root)));
        }
    }

    AVLTree &operator=(const AVLTree &src)
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

    inline void clear() { __root.reset(); }

    inline bool isEmpty() const { return __root.get() == nullptr; }

    inline int height() const { return __root->__height; }

    inline int getSize() const { return __size; }

    bool insert(const DATA_TYPE &data);
    bool remove(const DATA_TYPE &data);

private:
    struct Node
    {
        DATA_TYPE __data;
        Node_pointer __parent;
        Node_unique_ptr __left;
        Node_unique_ptr __right;
        int __height;

        Node(const DATA_TYPE &data) : __parent(nullptr), __data(data), __left(nullptr), __right(nullptr), __height(0) {}
        ~Node() = default;
        Node(const Node &src) : __parent(nullptr), __data(src.__data), __left(nullptr), __right(nullptr), __height(src.__height)
        {
            if (src.hasLeft())
            {
                setLeft(src.__left->__data);
            }
            if (src.hasRight())
            {
                setRight(src.__right->__data);
            }
        }

        void setLeft(const DATA_TYPE &data) // adds only one node to the left
        {
            __left.reset(new Node(data));
            __left->__parent = this;
            recalculateHeight();
        }

        void setRight(const DATA_TYPE &data)
        {
            __right.reset(new Node(data));
            __right->__parent = this;
            recalculateHeight();
        }

        void recalculateHeight() { __height = 1 + our::max((hasLeft()) ? (__left->__height) : (0), (hasRight()) ? (__right->__height) : (0)); }
        bool isLeaf() const { return __left == nullptr && __right == nullptr; }
        bool isRoot() const { return __parent == nullptr; }
        bool hasLeft() const { return !__left == nullptr; }
        bool hasRight() const { return !__right == nullptr; }

        int balanceFactor() const { return (hasLeft()) ? (__left->__height) : (0) - (hasRight()) ? (__right->__height)
                                                                                                 : (0); }
        inline bool isAVLBalanced() const { return our::abs(balanceFactor()) <= 1; }
    };
    Node_unique_ptr __root;
    int __size;

    // helper functions
    void recalculateAllNodesHeight
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
        Node_unique_ptr &pA = node_ptr;
        Node_unique_ptr &pB = pA->__left;
        Node_unique_ptr &pb = pB->__right;

        Node_unique_ptr temp;
        pb.swap(temp);
        pb.swap(pA);
        pB.swap(temp);
        pA.swap(temp);

        /**
         * now:
         * pA points to B
         * pB points to b
         * pb points to A
         */

        pA->__parent = pb->__parent; // B's parent points to A's previous parent
        pb->__parent = pA.get();   // B is the parent of A
        pB->__parent = pb.get();   // A is the parent of b

        pB->recalculateHeight();
        pb->recalculateHeight();
        pA->recalculateHeight();


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


        Node_unique_ptr &pA = node_ptr;
        Node_unique_ptr &pB = pA->__left;
        Node_unique_ptr &pb = pB->__right;
        Node_unique_ptr &pc = pB->__left;
        Node_unique_ptr &pa = pA->__right;


        Node_unique_ptr temp;
        pb.swap(temp);
        pb.swap(pA);
        pB.swap(temp);
        pA.swap(temp);

        /**
         * now:
         * pA points to B
         * pB points to b
         * pb points to A
         */

        pA->__parent = pb->__parent; // B's parent points to A's previous parent
        pb->__parent = pA.get();   // B is the parent of A
        pB->__parent = pb.get();   // A is the parent of b

        pB->recalculateHeight();
        pb->recalculateHeight();
        pA->recalculateHeight();

        return true;
    }
    Node_unique_ptr& getUniquePtr(const Node& node) const
    {
        if (node.isRoot())
            return __root;
        if (node.__parent->__left.get() == &node)
        {
            return node.__parent->__left;
        }
        else {
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
    bool swapWithLeftChild(Node_unique_ptr& node_ptr)
    {
        if (node_ptr.get() == nullptr || node_ptr->isLeaf() || !node_ptr->hasLeft())
        {
            return false;
        }
        Node_unique_ptr &pA = node_ptr;
        Node_unique_ptr &pB = pA->__left;
        Node_unique_ptr &pb = pB->__right;
        Node_unique_ptr &pc = pB->__left;
        Node_unique_ptr &pa = pA->__right;
        Node_pointer tempPtr;

        pa.swap(pb);
        pc.swap(temp);
        pc.swap(pA);
        pB.swap(temp);
        pA.swap(temp);

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
        if(pB != nullptr)
        {
            pB->parent = pc.get();
            pB->recalculateHeight();
        }
        if(pb != nullptr)
        {
            pb->parent = pA.get();
            pb->recalculateHeight();
        }
        if(pa != nullptr)
        {
            pa->parent = pc.get();
            pa->recalculateHeight();
        }
        // first update the height of A (cuz it on the bottom now)
        pc->recalculateHeight();
        // then update the height of B (cuz it on the top now)
        pA->recalculateHeight();

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
    bool swapWithRightChild(Node_unique_ptr& node_ptr)
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
        Node_pointer tempPtr;

        pa.swap(pc);
        pb.swap(temp);
        pb.swap(pA);
        pB.swap(temp);
        pA.swap(temp);

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
        if(pB != nullptr)
        {
            pB->parent = pb.get();
            pB->recalculateHeight();
        }
        if(pc != nullptr)
        {
            pc->parent = pA.get();
            pc->recalculateHeight();
        }
        if(pa != nullptr)
        {
            pa->parent = pb.get();
            pa->recalculateHeight();
        }
        // first update the height of A (cuz it on the bottom now)
        pb->recalculateHeight();
        // then update the height of B (cuz it on the top now)
        pA->recalculateHeight();

        return true;
    }

    void swap(Node_unique_ptr& first, Node_unique_ptr& second) 
    {
        if(first.get() != second.get()) // don't really need to check for self swap
        {
            if(first->__right == second.get())
            {
                swapWithRightChild(first);
            }
            if(first->__left == second.get())
            {
                swapWithLeftChild(first);
            }
            else if(second->__right == first.get())
            {
                swapWithRightChild(second);
            }
            else if(second->__left == first.get())
            {
                swapWithLeftChild(second);
            }
            else {
                first->__left.swap(second->__left); // swap left children
                first->__right.swap(second->__right); // swap right children
                first.swap(second); // swap the nodes themselves (what the unique ptrs point to)
                our::swap(first->__parent, second->__parent); // update the parents
                our::swap(first->__height, second->__height);
            }
        }
    }
};

template <typename DATA_TYPE>
bool AVLTree<DATA_TYPE>::insert(const DATA_TYPE &data)
{
    if (isEmpty())
    {
        __root.reset(new Node(data));
        return true;
    }
    Node_pointer tempNodePtr = __root.get();
    bool goLeft = false;
    while (!tempNodePtr->isLeaf())
    {
        if (tempNodePtr->__data == data)
        {
            return false;
        }
        else if (tempNodePtr->__data < data)
        {
            tempNodePtr = tempNodePtr->__left.get();
        }
        else if (tempNodePtr->__data > data)
        {
            tempNodePtr = tempNodePtr->__right.get();
        }
    }
    // check where to go at the leaf
    if (tempNodePtr->__data == data)
    {
        return false;
    }
    else if (tempNodePtr->__data < data)
    {
        tempNodePtr->setLeft(data);
    }
    else if (tempNodePtr->__data > data)
    {
        tempNodePtr->setRight(data);
    }
    // now the new data is added

    while (tempNodePtr != nullptr && tempNodePtr->isAVLBalanced())
    {
        tempNodePtr = tempNodePtr->__parent;
    }
    // tempNodePtr now points to an unbalanced tree, time to balance it
    // check if all tree is balanced
    if (tempNodePtr != nullptr) // tree is not balanced
    {
        if (tempNodePtr->balanceFactor() >= 2) // left heavy
        {
            if (tempNodePtr->__left->balanceFactor() >= 0) // LL rotation
            {
                rotateLeft(tempNodePtr->__left);
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
            if (tempNodePtr->getRightSubTreePtr()->balanceFactor() <= 0) // RR rotation
            {
                rotateRight(tempNodePtr->__right);
                rotateRight(getUniquePtr(*(tempNodePtr)));
            }
            else // balanceFactor == 1, RL rotation
            {
                rotateLeft(tempNodePtr->__right);
                rotateRight(getUniquePtr(*(tempNodePtr)));
            }
        }
    }
    return true;
}

template <typename DATA_TYPE>
bool AVLTree<DATA_TYPE>::remove(const DATA_TYPE &data)
{
    // search for the node(unique pointer)
    Node_pointer tempNodePtr = __root.get();
    while (tempNodePtr != nullptr)
    {
        if (tempNodePtr->__data == data)
        {
            break;
        }
        else if (tempNodePtr->__data < data)
        {
            tempNodePtr = tempNodePtr->__left.get();
        }
        else if (tempNodePtr->__data > data)
        {
            tempNodePtr = tempNodePtr->__right.get();
        }
    }
    if(tempNodePtr == nullptr)
    {
        return false;
    }
    // now tempNodePtr points to a (valid) node we want to remove
    Node_pointer toDeletePtr = tempNodePtr;
    bool hasLeft = toDeletePtr->hasLeft();
    bool hasRight = toDeletePtr->hasRight();
    if (hasLeft && hasRight)
    {
        toDeletePtr = toDeletePtr->__right.get();
        while (toDeletePtr->hasLeft())
        {
            toDeletePtr = toDeletePtr->__left.get();
        }
        swap(getUniquePtr(toDeletePtr), getUniquePtr(tempNodePtr));
    }
    Node_unique_ptr &toDeleteUniquePtr = getUniquePtr(toDeletePtr);
    if (!hasLeft && !hasRight) // leaf
    {
        toDeleteUniquePtr.reset();
    }
    if (hasLeft && !hasRight)
    {
        rotateLeft(toDeleteUniquePtr); // the node we want to delete is the right son of the node that toDeleteUniquePte points to
        toDeleteUniquePtr->__right.reset();
    }
    if (!hasLeft && hasRight)
    {
        rotateRight(toDeleteUniquePtr); // the node we want to delete is the left son of the node that toDeleteUniquePte points to
        toDeleteUniquePtr->__left.reset();
    }




    
    // now we should update the path in the AVL tree
    BinaryTree treePtr = next;
    while (treePtr != nullptr && !isTreeAVLBalanced(*treePtr))
    {
        if (treePtr->balanceFactor() >= 2) // left heavy
        {
            if (treePtr->getLeftSubTreePtr()->balanceFactor() >= 0) // LL rotation
            {
                treePtr->getLeftSubTreePtr()->rotateLeft();
                treePtr->rotateLeft();
            }
            else // balanceFactor == -1, LR rotation
            {
                treePtr->getLeftSubTreePtr()->rotateRight();
                treePtr->rotateLeft();
            }
        }
        if (treePtr->balanceFactor() <= -2) // right heavy
        {
            if (treePtr->getRightSubTreePtr()->balanceFactor() <= 0) // RR rotation
            {
                treePtr->getRightSubTreePtr()->rotateRight();
                treePtr->rotateRight();
            }
            else // balanceFactor == 1, RL rotation
            {
                treePtr->getRightSubTreePtr()->rotateLeft();
                treePtr->rotateRight();
            }
        }
        treePtr = treePtr->getParent();
    }
}
