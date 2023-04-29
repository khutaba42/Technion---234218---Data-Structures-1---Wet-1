#ifndef _AVL_TREE_H_
#define _AVL_TREE_H_

// library includes
#include <memory> // for unique_ptr
#include <math.h>
#include <exception>

// our includes
#include "ourUtilityFunctions.h"

/* --------------- -*- C++ AVL TREE -*- --------------- */
template <typename DATA_TYPE>
class AVLTree;

template <typename DATA_TYPE>
class AVLTree
{
public:
private:
    class BinaryTree
    {
    public:
        BinaryTree() = default;
        ~BinaryTree() = default;
        BinaryTree(const BinaryTree &src) : __root_ptr(new Node(src.getRootPtr())) {}
        BinaryTree &operator=(const BinaryTree &src);

        void clear() { clear_aux(); }
        bool isEmpty() const { return getRootPtr() == nullptr; }
        int Height() const { return getRootPtr()->getHeight(); }

        void swap(BinaryTree &other) { __root_ptr.swap(other.__root_ptr); }
        void swapChildren() { getRootPtr()->swapChildren(); }

        void swapLeftChildren(BinaryTree &other) { getLeftSubTree().swap(other.getLeftSubTree()); }
        void swapRightChildren(BinaryTree &other) { getRightSubTree().swap(other.getRightSubTree()); }

        void deleteLeftSubTree() { getLeftSubTree().clear(); }
        void deleteRightSubTree() { getRightSubTree().clear(); }

        bool insertRootTurnLeft(DATA_TYPE &data);
        bool insertRootTurnRight(DATA_TYPE &data);

        bool rotateLeft();
        bool rotateRight();

    private:
        class Node
        {
        public:
            Node(DATA_TYPE &data);
            ~Node() = default;
            Node(const Node &src) : __parent(nullptr), __height(src.getHeight()), __data(src.getData()), __left(src.__left), __right(src.__right)
            {
                __left.__root_ptr->setParent(this->__left);
                __right.__root_ptr->setParent(this->__right);
            }
            Node &operator=(const Node &src);

            inline bool isLeaf() const { return !hasLeftChild() && !hasRightChild(); }
            inline int getHeight() const { return __height; }
            inline const DATA_TYPE &getData() const { return __data; }
            inline void swapChildren() { __left.swap(__right); }
            bool hasLeftChild() const { return !__left.isEmpty(); }
            bool hasRightChild() const { return !__right.isEmpty(); }
            void setParent(const BinaryTree &newParent) { __parent = &newParent; }

        private:
            BinaryTree *__parent;
            int __height;
            DATA_TYPE __data;
            BinaryTree __left, __right;
            // Node Helper function

            BinaryTree &get
        };

        std::unique_ptr<Node> __root_ptr;

        // Binary Tree helper functions
        inline void clear_aux() { __root_ptr.reset(); }
        inline Node *getRootPtr() const { return __root_ptr.get(); }
        inline BinaryTree &getLeftSubTree() { return __root_ptr->__left; }
        inline BinaryTree &getRightSubTree() { return __root_ptr->_right; }
        void setRoot(Node *root)
        {
        }
    };
};

#endif
