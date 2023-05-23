#ifndef _AVL_TREE_H_
#define _AVL_TREE_H_

#include <string>
#include <iostream>
#include "stack.h"
#include <exception>

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

template <typename T, Comparison (*compFunction)(const T &, const T &) = AVLTree_CompareUsingOperators<T>>
class AVLTree
{
    struct AVLTreeNode
    {
        AVLTreeNode *__left;
        AVLTreeNode *__right;

        T __value;
        int __count; // how many nodes are there in this subtree
        int __height;

        AVLTreeNode(T value) : __value(value)
        {
            __count = 1;
            __height = 1;

            __left = nullptr;
            __right = nullptr;
        }

        void updateValues()
        {
            __count = ((__left != nullptr) ? __left->__count : 0) + ((__right != nullptr) ? __right->__count : 0) + 1;

            __height = std::max(__left != nullptr ? __left->__height : 0,
                              __right != nullptr ? __right->__height : 0) +
                     1;
        }

        int balanceFactor()
        {
            return (__left != nullptr ? __left->__height : 0) - (__right != nullptr ? __right->__height : 0);
        }

        AVLTreeNode *left_rotate()
        {
            AVLTreeNode *R = __right;
            __right = __right->__left;
            R->__left = this;

            this->updateValues(); // the order is important
            R->updateValues();

            return R;
        }

        AVLTreeNode *right_rotate()
        {
            AVLTreeNode *L = __left;
            __left = __left->__right;
            L->__right = this;

            this->updateValues(); // the order is important
            L->updateValues();

            return L;
        }
    };
    int __size;
    AVLTreeNode *__root;

    void balance(Stack<AVLTreeNode **> &path);
    void display(AVLTreeNode *cur, int depth = 0, int state = 0);

    template <typename FunctionObject>
    bool in_order_traversal_aux_recursive(AVLTreeNode *const root, FunctionObject do_something) const
    {
        if (root == nullptr)
        {
            return false;
        }
        in_order_traversal_aux_recursive(root->__left, do_something);
        do_something(root->__value);
        in_order_traversal_aux_recursive(root->__right, do_something);
        return true;
    }

    template <typename FunctionObject>
    bool reverse_in_order_traversal_aux_recursive(AVLTreeNode *const root, FunctionObject do_something) const
    {
        if (root == nullptr)
        {
            return false;
        }
        reverse_in_order_traversal_aux_recursive(root->__right, do_something);
        do_something(root->__value);
        reverse_in_order_traversal_aux_recursive(root->__left, do_something);
        return true;
    }

public:
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

    AVLTree();
    ~AVLTree();

    void insert(T value);
    void remove(T value);

    void clear();
    bool isEmpty() const;
    int getSize() const;

    T &find(T value);
    const T &find(T value) const;
    const T &getMin() const;
    const T &getMax() const;
    T &operator[](std::size_t idx);
    const T &operator[](std::size_t idx) const;

    template <typename FunctionObject>
    void in_order_traversal(FunctionObject do_something) const
    {
        in_order_traversal_aux_recursive(__root, do_something);
    }

    template <typename FunctionObject>
    void reverse_in_order_traversal(FunctionObject do_something)
    {
        reverse_in_order_traversal_aux_recursive(__root, do_something);
    }

    void display();
};

template <class T, Comparison (*compFunction)(const T &, const T &)>
AVLTree<T, compFunction>::AVLTree()
{
    __root = nullptr;
    __size = 0;
}

template <class T, Comparison (*compFunction)(const T &, const T &)>
AVLTree<T, compFunction>::~AVLTree()
{
    clear();
}

template <class T, Comparison (*compFunction)(const T &, const T &)>
void AVLTree<T, compFunction>::clear()
{
    Stack<AVLTreeNode *> stack;

    if (__root != nullptr)
        stack.push_back(__root);

    while (!stack.isEmpty())
    {
        AVLTreeNode *node = stack.back();
        stack.pop_back();

        if (node->__left != nullptr)
            stack.push_back(node->__left);

        if (node->__right != nullptr)
            stack.push_back(node->__right);

        __size--;
        delete node;
        node = nullptr; // Set the pointer to nullptr after deletion
    }

    __root = nullptr;
}

template <class T, Comparison (*compFunction)(const T &, const T &)>
void AVLTree<T, compFunction>::insert(T value)
{
    AVLTreeNode **indirect = &__root;
    Stack<AVLTreeNode **> path;

    while (*indirect != nullptr)
    {
        path.push_back(indirect);

        Comparison result = compFunction(value, (*indirect)->__value);
        if (result == Comparison::less)
            indirect = &((*indirect)->__left);
        else if (result == Comparison::greater)
            indirect = &((*indirect)->__right);
        else if (result == Comparison::equal)
            throw ElementAlreadyExistsException();
    }

    *indirect = new AVLTreeNode(value);
    path.push_back(indirect);

    balance(path);
    __size++;
}

template <class T, Comparison (*compFunction)(const T &, const T &)>
void AVLTree<T, compFunction>::remove(T value)
{
    AVLTreeNode **indirect = &__root; // to generalize insertion
    Stack<AVLTreeNode **> path;       // to update height values

    /*
    Searching for the Node:
    The function starts by searching for the node containing the value from the root of the tree.
    It keeps a pointer to the node (indirect) and records the path to the node in a stack (path) for later use.
    If the node is not found, it throws a NoSuchElementException.
    */
    while (*indirect != nullptr)
    {
        path.push_back(indirect);

        Comparison result = compFunction(value, (*indirect)->__value);
        if (result == Comparison::less)
            indirect = &((*indirect)->__left);
        else if (result == Comparison::greater)
            indirect = &((*indirect)->__right);
        else // if (result == Comparison::equal)
            break;
    }

    if (*indirect == nullptr) // the value does not exist in tree
        throw NoSuchElementException();

    else
        path.push_back(indirect);

    std::size_t index = path.size();

    /*
    Deletion of the Node:
    After finding the node, it checks the number of children the node has and proceeds accordingly:

    If the node has no children (it is a leaf), it simply deletes the node and sets the pointer to it (*indirect) to nullptr.
    If the node has only a left child, it replaces the node with its left child and then deletes the node.
    If the node has a right child, it finds the node's in-order successor (the node with the smallest value that is larger than the node's value,
    which is the left-most node in its right sub-tree). It then replaces the node with its in-order successor and deletes the node.
    */
    if ((*indirect)->__left == nullptr and (*indirect)->__right == nullptr)
    {                     // the node is leaf
        delete *indirect; // just delete node
        *indirect = nullptr;
        path.pop_back(); // pop the deleted node from path
    }

    else if ((*indirect)->__right == nullptr)
    { // only left child exists
        AVLTreeNode *toRemove = *indirect;

        (*indirect) = (*indirect)->__left;
        delete toRemove;

        // path.pop_back();
    }

    else
    { // right child exists
        AVLTreeNode **successor = &((*indirect)->__right);

        while ((*successor)->__left != nullptr)
        {
            path.push_back(successor);
            successor = &((*successor)->__left);
        }

        if (*successor == (*indirect)->__right)
        {
            (*successor)->__left = (*indirect)->__left;

            AVLTreeNode *toRemove = *indirect;
            *indirect = *successor;
            delete toRemove;
        }

        else
        {
            AVLTreeNode *tmp = *path.back(), *suc = *successor;

            tmp->__right = suc->__right;
            suc->__left = (*indirect)->__left;
            suc->__right = (*indirect)->__right;

            delete *indirect;
            *indirect = suc;
            path[index] = &(suc->__right);
        }
    }

    balance(path);
    __size--;
}

template <class T, Comparison (*compFunction)(const T &, const T &)>
void AVLTree<T, compFunction>::balance(Stack<AVLTreeNode **> &path)
{
    AVLTreeNode **indirect;
    while (!path.isEmpty())
    {
        indirect = path.back();
        path.pop_back();
        if (*indirect == nullptr)
        {
            continue;
        }

        (*indirect)->updateValues();

        if ((*indirect)->balanceFactor() >= 2 and (*indirect)->__left->balanceFactor() >= 0) // left - left
            *indirect = (*indirect)->right_rotate();

        else if ((*indirect)->balanceFactor() >= 2)
        { // left - right
            (*indirect)->__left = (*indirect)->__left->left_rotate();
            *indirect = (*indirect)->right_rotate();
        }

        else if ((*indirect)->balanceFactor() <= -2 and (*indirect)->__right->balanceFactor() <= 0) // right - right
            *indirect = (*indirect)->left_rotate();

        else if ((*indirect)->balanceFactor() <= -2)
        { // right - left
            (*indirect)->__right = ((*indirect)->__right)->right_rotate();
            *indirect = (*indirect)->left_rotate();
        }
    }
}

template <class T, Comparison (*compFunction)(const T &, const T &)>
bool AVLTree<T, compFunction>::isEmpty() const
{
    return __size == 0;
}

template <class T, Comparison (*compFunction)(const T &, const T &)>
int AVLTree<T, compFunction>::getSize() const
{
    return __size;
}

template <class T, Comparison (*compFunction)(const T &, const T &)>
T &AVLTree<T, compFunction>::find(T value)
{
    AVLTreeNode *direct = __root;
    while (direct != nullptr)
    {
        Comparison result = compFunction(value, direct->__value);
        if (result == Comparison::equal)
        {
            return direct->__value;
        }
        else if (result == Comparison::less)
        {
            direct = direct->__left;
        }
        else if (result == Comparison::greater)
        {
            direct = direct->__right;
        }
    }
    throw NoSuchElementException();
}

template <class T, Comparison (*compFunction)(const T &, const T &)>
const T &AVLTree<T, compFunction>::find(T value) const
{
    AVLTreeNode *direct = __root;
    while (direct != nullptr)
    {
        Comparison result = Comparison::equal;
        result = compFunction(value, direct->__value);
        if (result == Comparison::equal)
        {
            return direct->__value;
        }
        else if (result == Comparison::less)
        {
            direct = direct->__left;
        }
        else if (result == Comparison::greater)
        {
            direct = direct->__right;
        }
    }
    throw NoSuchElementException();
}

template <class T, Comparison (*compFunction)(const T &, const T &)>
const T &AVLTree<T, compFunction>::getMin() const
{
    AVLTreeNode *cur = __root;

    while (cur->__left != nullptr)
        cur = cur->__left;

    return cur->__value;
}

template <class T, Comparison (*compFunction)(const T &, const T &)>
const T &AVLTree<T, compFunction>::getMax() const
{
    AVLTreeNode *cur = __root;

    while (cur->__right != nullptr)
        cur = cur->__right;

    return cur->__value;
}

template <class T, Comparison (*compFunction)(const T &, const T &)>
const T &AVLTree<T, compFunction>::operator[](std::size_t idx) const
{
    AVLTreeNode *cur = __root;
    int left = cur->__left != nullptr ? cur->__left->count : 0;

    while (left != idx)
    {
        if (left < idx)
        {
            idx -= left + 1;

            cur = cur->__right;
            left = cur->__left != nullptr ? cur->__left->count : 0;
        }

        else
        {
            cur = cur->__left;
            left = cur->__left != nullptr ? cur->__left->count : 0;
        }
    }

    return cur->__value;
}

template <class T, Comparison (*compFunction)(const T &, const T &)>
void AVLTree<T, compFunction>::display()
{
    printf("\n");
    if (__root != nullptr)
        display(__root);
    else
        printf("Empty");
    printf("\n");
}

template <class T, Comparison (*compFunction)(const T &, const T &)>
void AVLTree<T, compFunction>::display(AVLTreeNode *cur, int depth, int state)
{ // state: 1 -> left, 2 -> right , 0 -> root
    if (cur->__left)
        display(cur->__left, depth + 1, 1);

    for (int i = 0; i < depth; i++)
        printf("     ");

    if (state == 1) // left
        printf("┌───");
    else if (state == 2) // right
        printf("└───");

    std::cout << "[" << cur->__value << "]" << /*- (" << cur->count << ", " << cur->height << ")" << */ std::endl;

    if (cur->__right)
        display(cur->__right, depth + 1, 2);
}

#endif // _AVL_TREE_H_
