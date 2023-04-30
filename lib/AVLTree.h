#ifndef _AVL_TREE_H_
#define _AVL_TREE_H_

// library includes
#include <memory> // for unique_ptr
#include <math.h>
#include <exception>

// our includes
#include "ourUtilityFunctions.h"

/* --------------- -*- C++ AVL TREE -*- --------------- */
/**
 * this data structure assumes that DATA_TYPE has the operators < > =
 */
template <typename DATA_TYPE>
class AVLTree;
// HI
template <typename DATA_TYPE>
class AVLTree
{
public:
    AVLTree() = default;
    ~AVLTree() = default;
    AVLTree(const AVLTree &src);
    AVLTree &operator=(const AVLTree &src);

    bool isEmpty() const { return __tree.isEmpty(); }

    bool insert(const DATA_TYPE &data);
    void remove(const DATA_TYPE &data);

    int getHeight() const;
    int getSize() const;
/*
    // in-order traversal
    class Iterator
    {
    public:
        // error classes
        class InvalidOperation
        {
        };

        // operators
        DATA_TYPE &operator*() const { return __currTreePtr->getRootPtr()->getData(); }
        Iterator &operator++();
        bool operator!=(const Iterator &other) const {return __currTreePtr != other.__currTreePtr;}

        // copy c'tor
        Iterator(const Iterator &) = default;

        // assignment operator
        Iterator &operator=(const Iterator &) = default;

        // d'tor
        ~Iterator() = default;

    private:
        BinaryTree* __currTreePtr;
        friend class AVLTree; // this allows AVLTree to use the Iterator's c'tor
        // c'tor
        Iterator(const AVLTree *tree) : __currTreePtr(tree->__tree) {}
    };
    Iterator begin();
    Iterator end();
*/

private:
    friend class Iterator;
    class BinaryTree
    {
    public:
        BinaryTree() = default;
        ~BinaryTree() = default;
        BinaryTree(const BinaryTree &src) : __root_ptr((src.isEmpty())
                                                           ? (nullptr)
                                                           : (new Node(*(src.getRootPtr())))) /* if the src isn't empty we can access the root, otherwise a disaster */
        /* added a termination if statement, otherwise it would have been a recursive infinite loop */ {}
        BinaryTree &operator=(const BinaryTree &src);

        void clear() { clear_aux(); }
        bool isEmpty() const { return getRootPtr() == nullptr; }
        int getHeight() const { return getRootPtr()->getHeight(); }
        inline const DATA_TYPE& getRootData() {return getRootPtr()->getData();}
        void swap(BinaryTree &other) { __root_ptr.swap(other.__root_ptr); }
        void swapChildren() { getRootPtr()->swapChildren(); }

        void swapRoots(BinaryTree &other);

        void swapLeftChildren(BinaryTree &other) { getLeftSubTreePtr()->swap(other->getLeftSubTreePtr()); }
        void swapRightChildren(BinaryTree &other) { getRightSubTreePtr()->swap(other->getRightSubTreePtr()); }

        void deleteLeftSubTree() { getLeftSubTreePtr()->clear(); }
        void deleteRightSubTree() { getRightSubTreePtr()->clear(); }

        bool insertRootToLeft(DATA_TYPE &data);
        bool insertRootToRight(DATA_TYPE &data);

        bool rotateLeft();
        bool rotateRight();

        bool hasRightChild() const{
            if(isEmpty())
                return false;
            return getRootPtr()->hasRightChild();
        }
        bool hasLeftChild() const{
            if(isEmpty())
                return false;
            return getRootPtr()->hasLeftChild();
        }

        inline int balanceFactor() const
        {
            if (isEmpty())
                return 0;
            return getLeftSubTreePtr()->getHeight() - getRightSubTreePtr()->getHeight();
        }
        bool isAVLBalanced() const { return our::abs(balanceFactor()) <= 1; }

    private:
        class Node
        {
        public:
            Node(DATA_TYPE &data);
            ~Node() = default;
            Node(const Node &src) : __height(src.getHeight()), __data(src.getData()), __left(src.__left), __right(src.__right) {}
            Node &operator=(const Node &src);

            inline bool isLeaf() const { return !hasLeftChild() && !hasRightChild(); }
            inline int getHeight() const { return __height; }
            inline const DATA_TYPE &getData() const { return __data; }
            inline void swapChildren() { __left.swap(__right); }
            bool hasLeftChild() const { return !__left.isEmpty(); }
            bool hasRightChild() const { return !__right.isEmpty(); }

        private:
            DATA_TYPE __data;
            BinaryTree __left, __right;
            // Node Helper function

        };
        BinaryTree *__parent;
        std::unique_ptr<Node> __root_ptr;
        int __size;
        int __height;
        // Binary Tree helper functions
        
        inline void clear_aux() { __root_ptr.reset(); }
        inline Node *getRootPtr() const { return __root_ptr.get(); }
        
        inline BinaryTree *getLeftSubTreePtr() { return &__root_ptr->__left; }
        inline BinaryTree *getRightSubTreePtr() { return &__root_ptr->_right; }

        void setLeftChild(BinaryTree* leftChild)
        {
            getRootPtr()->__right = leftChild;
        }

        inline BinaryTree *getParent() { return __parent; }
        inline void setParent(const BinaryTree *parentPtr) { __parent = parentPtr; }
        void setRoot(Node *root)
        {
        }
    };
    BinaryTree __tree;

    class NoSuchElementException : std::exception
    {
        const char *what() const noexcept override { return "there is no such element as provided"; };
    };
    // AVL helper function

    BinaryTree &
    search(const DATA_TYPE &data) const
    {
        BinaryTree *treePtr = &__tree;
        while (treePtr->isEmpty())
        {
            if (treePtr->getRootPtr()->getData() == data)
            {
                return *treePtr;
            }
            if (treePtr->getRootPtr()->getData() < data)
            {
                treePtr = treePtr->getLeftSubTreePtr();
            }
            if (treePtr->getRootPtr()->getData() > data)
            {
                treePtr = treePtr->getRightSubTreePtr();
            }
        }
        throw NoSuchElementException();
    }
};

template <typename DATA_TYPE>
bool AVLTree<DATA_TYPE>::insert(const DATA_TYPE &data)
{
    BinaryTree *treePtr = &__tree;
    bool goLeft = false;
    while (treePtr->isEmpty())
    {
        if (treePtr->getRootPtr()->getData() == data)
        {
            return false;
        }
        if (treePtr->getRootPtr()->getData() < data)
        {
            treePtr = treePtr->getLeftSubTreePtr();
        }
        if (treePtr->getRootPtr()->getData() > data)
        {
            treePtr = treePtr->getRightSubTreePtr();
        }
    }
    treePtr->insertRootToLeft(data); // direction doesn't matter
    while (isTreeAVLBalanced(*treePtr))
    {
        treePtr = treePtr->getParent();
    }
    // treePtr now points to an unbalanced tree, time to balance it
    // check if all tree is balanced
    if (treePtr != nullptr) // tree is not balanced
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
    }
    return true;
}

template <typename DATA_TYPE>
void AVLTree<DATA_TYPE>::remove(const DATA_TYPE &data)
{
    BinaryTree &toDelete = search(data);
    bool hasLeft = toDelete.hasLeftChild();
    bool hasRight = toDelete.hasRightChild();
    if (!hasLeft && !hasRight) // leaf
    {
        toDelete.clear();
    }
    if(hasLeft && !hasRight)
    {
        toDelete.rotateLeft();
        toDelete.clear();
    }
    if(!hasLeft && hasRight)
    {
        toDelete.rotateRight();
        toDelete.clear();
    }
    BinaryTree* next = &toDelete;
    if(hasLeft && hasRight)
    {
        next = toDelete.getRightSubTreePtr();
        while(next->hasLeftChild())
        {
            next = next->getLeftSubTreePtr();
        }
        toDelete.swapRoots(*next);

        bool hasLeft = next->hasLeftChild();
        bool hasRight = next->hasRightChild();
        if (!hasLeft && !hasRight) // leaf
        {
            next->clear();
        }
        if(hasLeft && !hasRight)
        {
            next->rotateLeft();
            next->clear();
        }
        if(!hasLeft && hasRight)
        {
            next->rotateRight();
            next->clear();
        }
        // shouldn't get here
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

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                                                            //
//                                                                            //
//                                 Iterator                                   //
//                                                                            //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*
template <class DATA_TYPE>
typename AVLTree<DATA_TYPE>::Iterator &AVLTree<DATA_TYPE>::Iterator::operator++()
{
    if (__currTreePtr->hasLeftSubTree())
    {
        
    }
    
}*/


#endif
