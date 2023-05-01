
#include <memory>
#include "ourUtilityFunctions.h"

// comment out for no testing
#define TESTING


#ifdef TESTING
#include <string>
namespace testing {
std::string setw(int width) {
    std::string spaces;
    for (int i = 0; i < width; i++) {
        spaces += " ";
    }
    return spaces;
}
};
#endif

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
        const char *what() const noexcept override { return "There is no such element"; }
    };
    class ElementAlreadyExistsException : public std::exception
    {
        const char *what() const noexcept override { return "Element already exists"; }
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

    inline void clear()
    {
        __root.reset();
        __size = 0;
    }

    inline bool isEmpty() const { return __root.get() == nullptr; }

    inline int height() const { return __root->__height; }

    inline int getSize() const { return __size; }

    bool insert(const DATA_TYPE &data);
    bool remove(const DATA_TYPE &data);
#ifdef TESTING

void print() const;

void printNode(const Node_pointer &node_ptr, int depth) const;

#endif

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
            Node_pointer temp = this;
            while(temp != nullptr)
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
            while(temp != nullptr)
            {
                temp->recalculateHeight();
                temp = temp->__parent;
            }
        }

        void recalculateHeight() { __height = 1 + our::max((hasLeft()) ? (__left->__height) : (-1), (hasRight()) ? (__right->__height) : (-1)); }
        bool isLeaf() const { return ((__left == nullptr) && (__right == nullptr)); }
        bool isRoot() const { return (__parent == nullptr); }
        bool hasLeft() const { return (!(__left == nullptr)); }
        bool hasRight() const { return (!(__right == nullptr)); }

        int balanceFactor() const { return ((hasLeft()) ? (__left->__height) : (-1)) - ((hasRight()) ? (__right->__height)
                                                                                                    : (-1)); }
        inline bool isAVLBalanced() const { return our::abs(balanceFactor()) <= 1; }
    };
    Node_unique_ptr __root;
    int __size;

    // helper functions

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

        Node_unique_ptr temp_ptr;
        pb.swap(temp_ptr);
        pb.swap(pA);
        pB.swap(temp_ptr);
        pA.swap(temp_ptr);

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
            pB->__parent = pb.get();     // A is the parent of b
        }
        
        

        Node_pointer temp = pb.get();

        while (temp)
        {
            temp->recalculateHeight();
            temp = temp->__parent;
        }

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
        if (node_ptr.get() == nullptr || node_ptr->isLeaf() || !node_ptr->hasRight() )
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
        if(pB.get() != nullptr)
        {
            pB->__parent = pb.get(); 
        }    // A is the parent of b

        Node_pointer temp = pb.get();
        while (temp)
        {
            temp->recalculateHeight();
            temp = temp->__parent;
        }

        return true;
    }
    Node_unique_ptr &getUniquePtr(const Node &node)
    {
        if (node.isRoot())
            return __root;
        if (node.__parent->__left.get() == &node)
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

    void swap(Node_unique_ptr &first, Node_unique_ptr &second)
    {
        if (first.get() != second.get()) // don't really need to check for self swap
        {
            if (first->__right.get() == second.get())
            {
                swapWithRightChild(first);
            }
            if (first->__left.get() == second.get())
            {
                swapWithLeftChild(first);
            }
            else if (second->__right.get() == first.get())
            {
                swapWithRightChild(second);
            }
            else if (second->__left.get() == first.get())
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
};


template <typename DATA_TYPE>
bool AVLTree<DATA_TYPE>::insert(const DATA_TYPE &data)
{
    if (isEmpty())
    {
        __root.reset(new Node(data));
        __size++;
        return true;
    }
    Node_pointer tempNodePtr = __root.get();
    while (!tempNodePtr->isLeaf())
    {
        if (tempNodePtr->__data == data)
        {
            return false;
        }
        else if (tempNodePtr->__data > data)
        {
            tempNodePtr = tempNodePtr->__left.get();
        }
        else if (tempNodePtr->__data < data)
        {
            tempNodePtr = tempNodePtr->__right.get();
        }
    }
    // check where to go at the leaf
    if (tempNodePtr->__data == data)
    {
        return false;
    }
    else if (tempNodePtr->__data > data)
    {
        tempNodePtr->setLeft(data);
    }
    else if (tempNodePtr->__data < data)
    {
        tempNodePtr->setRight(data);
    }
    // now the new data is added

    while ((tempNodePtr != nullptr) && (tempNodePtr->isAVLBalanced()))
    {
        tempNodePtr = tempNodePtr->__parent;
    }
    if(tempNodePtr != nullptr)
    {
        if (tempNodePtr->balanceFactor() >= 2) // left heavy
        {
            if (tempNodePtr->__left->balanceFactor() >= 0) // LL rotation
            {
                //rotateLeft(tempNodePtr->__left);
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
                //rotateRight(tempNodePtr->__right);
                rotateRight(getUniquePtr(*(tempNodePtr)));
            }
            else // balanceFactor == 1, RL rotation
            {
                rotateLeft(tempNodePtr->__right);
                rotateRight(getUniquePtr(*(tempNodePtr)));
            }
        }
    }
    __size++;
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
        else if (tempNodePtr->__data > data)
        {
            tempNodePtr = tempNodePtr->__left.get();
        }
        else if (tempNodePtr->__data < data)
        {
            tempNodePtr = tempNodePtr->__right.get();
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
        toDeletePtr = toDeletePtr->__right.get();
        while (toDeletePtr->hasLeft())
        {
            toDeletePtr = toDeletePtr->__left.get();
        }
        swap(getUniquePtr(*toDeletePtr), getUniquePtr(*tempNodePtr));
    }
    Node_unique_ptr &toDeleteUniquePtr = getUniquePtr(*toDeletePtr);
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
    Node_pointer temp = toDeleteUniquePtr.get();
    while (temp != nullptr && !temp->isAVLBalanced())
    {
        if (temp->balanceFactor() >= 2) // left heavy
        {
            if (temp->__left->balanceFactor() >= 0) // LL rotation
            {
                //rotateLeft(temp->__left);
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
                //rotateRight(temp->__right);
                rotateRight(getUniquePtr(*temp));
            }
            else // balanceFactor == 1, RL rotation
            {
                rotateLeft(temp->__right);
                rotateRight(getUniquePtr(*temp));
            }
        }
        temp = temp->__parent;
    }
    __size--;
    return true;
}















































#ifdef TESTING
template <typename DATA_TYPE>
void AVLTree<DATA_TYPE>::print() const
{
    std::cout << "AVLTree size: " << __size << std::endl;
    printNode(__root.get(), 0);
}


template <typename DATA_TYPE>
void AVLTree<DATA_TYPE>::printNode(const Node_pointer &node_ptr, int level) const
{
    if (node_ptr == nullptr)
    {
        return;
    }

    printNode(node_ptr->__right.get(), level + 1);

    std::cout << testing::setw(level * 4) << ""; // adjust indentation based on level
    std::cout << "+--" << node_ptr->__data << std::endl;

    printNode(node_ptr->__left.get(), level + 1);
}
#endif
