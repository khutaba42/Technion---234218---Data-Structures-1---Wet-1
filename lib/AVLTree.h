#ifndef _AVL_TREE_H_
#define _AVL_TREE_H_

#include "stack.h"
#include "ourUtilityFunctions.h"
#include <iostream>

#include <cassert>

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

template <typename DATA_t, Comparison (*compFunction)(const DATA_t &, const DATA_t &) = AVLTree_CompareUsingOperators<DATA_t>>
class AVLTree
{
public:
    struct Node
    {
        DATA_t __data;
        Node *__left, *__right;
        int __height;

        Node(DATA_t data) : __data(data),
                            __left(nullptr),
                            __right(nullptr),
                            __height(0)
        {
        }

        void updateValues()
        {
            __height = 1 + our::max(__left != nullptr ? __left->__height : -1,
                                    __right != nullptr ? __right->__height : -1);
        }

        int balanceFactor()
        {
            return (__left != nullptr ? __left->__height : -1) - (__right != nullptr ? __right->__height : -1);
        }

        bool isLeaf() const
        {
            return ((__left == nullptr) && (__right == nullptr));
        }

        bool hasLeft() const
        {
            return __left != nullptr;
        }

        bool hasRight() const
        {
            return __right != nullptr;
        }

        // returns a pointer to the node that "replaced" the previous after rotation
        Node *left_rotate()
        {
            Node *R = __right;
            __right = __right->__left;
            R->__left = this;

            this->updateValues(); // the order is important
            R->updateValues();

            return R;
        }

        // returns a pointer to the node that "replaced" the previous after rotation
        Node *right_rotate()
        {
            Node *L = __left;
            __left = __left->__right;
            L->__right = this;

            this->updateValues(); // the order is important
            L->updateValues();

            return L;
        }

        friend void swapNodes(Node *&first, Node *&second)
        {
            if ((first == nullptr) || (second == nullptr) || (/*self swap bug (?)*/ first == second))
            {
                return;
            }
            our::swap(first->__left, second->__left);     // switch lefts
            our::swap(first->__right, second->__right);   // switch rights
            our::swap(first->__height, second->__height); // switch heights
            our::swap(first, second);                     // switch places (parents in tree)
        }
    };

    AVLTree();
    ~AVLTree();

    void insert(DATA_t data);
    void remove(DATA_t data);

    void clear();
    bool isEmpty() const;
    int getSize() const;

    DATA_t &find(const DATA_t &data);
    const DATA_t &find(const DATA_t &data) const;
    const DATA_t &getMin() const;
    const DATA_t &getMax() const;

    template <typename FunctionObject>
    void in_order_traversal(FunctionObject do_something)
    {
        in_order_traversal_aux_recursive(__root, do_something);
    }

    template <typename FunctionObject>
    void reverse_in_order_traversal(FunctionObject do_something)
    {
        reverse_in_order_traversal_aux_recursive(__root, do_something);
    }

    void display();

    // error classes
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

private:
    Node *__root;
    Node *__min_element;
    Node *__max_element;
    int __size;

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
       +---------------------------------------------------------------+
       |                                                               |
       |                        AVLTree Helper                         |
       |                                                               |
       +---------------------------------------------------------------+
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

    // deletes the tree with a pointer to the root, return teh size of tree it deleted
    int clear_aux(Node *root)
    {
        if (root == nullptr)
        {
            return 0;
        }
        int size = 1 + clear_aux(root->__left) + clear_aux(root->__right);
        delete root;
        return size;
    }

    // return ????????
    Node **find_node(DATA_t data)
    {
        Node **temp = &__root;
        while (*temp != nullptr)
        {
            Comparison result = compFunction(data, (*temp)->__data);
            if (result == Comparison::less)
            {
                temp = &((*temp)->__left);
            }
            else if (result == Comparison::greater)
            {
                temp = &((*temp)->__right);
            }
            else
            {
                return temp;
            }
        }
        throw NoSuchElementException();
    }

    Stack<Node *&> &find_path(DATA_t data, Stack<Node *&> &empty_path)
    {
        Node **temp = &__root;
        while ((*temp) != nullptr)
        {
            empty_path.push_back(*temp);
            Comparison result = compFunction(data, (*temp)->__data);
            if (result == Comparison::less)
            {
                temp = &((*temp)->__left);
            }
            else if (result == Comparison::greater)
            {
                temp = &((*temp)->__right);
            }
            else
            {
                break;
            }
        }
        if ((*temp) == nullptr)
        {
            empty_path.push_back(*temp);
        }

        return empty_path;
    }

    // return a pointer the node holding the min data
    Node *findMin() const
    {
        Node *temp = __root;
        if (temp == nullptr)
            throw NoSuchElementException();
        while (temp->__left != nullptr)
        {
            temp = temp->__left;
        }
        return temp;
    }

    // return a pointer the node holding the max data
    Node *findMax() const
    {
        Node *temp = __root;
        if (temp == nullptr)
            throw NoSuchElementException();
        while (temp->__right != nullptr)
        {
            temp = temp->__right;
        }
        return temp;
    }

    void balance(Stack<Node *&> &path)
    {
        // std::cout << "brfore balancing\n";
        // display();
        Node *curr = nullptr;
        while (!path.isEmpty())
        {
            // display();
            curr = path.back();
            // assert(curr != nullptr);
            // if (curr == nullptr)
            //{
            //    path.pop_back();
            //    continue;
            //}
            if (curr == nullptr)
            {

                path.pop_back();
                continue;
            }

            Node *&curr_reference = path.back();
            path.pop_back();
            // update height
            curr->updateValues();

            if (curr->balanceFactor() >= 2 && curr->__left->balanceFactor() >= 0)
            { // left - left
                curr_reference = curr->right_rotate();
            }
            else if (curr->balanceFactor() >= 2)
            { // left - right
                curr_reference->__left = curr->__left->left_rotate();
                curr_reference = curr->right_rotate();
            }

            else if (curr->balanceFactor() <= -2 and curr->__right->balanceFactor() <= 0)
            { // right - right
                curr_reference = curr->left_rotate();
            }
            else if (curr->balanceFactor() <= -2)
            { // right - left
                curr_reference->__right = curr->__right->right_rotate();
                curr_reference = curr->left_rotate();
            }
        }
        // std::cout << "after balancing\n";
        // display();
    }
    /**
     * @return -    true if insertion is successful
     *         -    false otherwise (duplicates)
     */
    bool insert_aux(DATA_t data)
    {
        Stack<Node *&> path;
        find_path(data, path);
        if (path.back() != nullptr)
        { // duplicate
            return false;
        }
        path.back() = new Node(data); // beware of bad_alloc
        path.pop_back();              // new inserted node dont need balancing
        // balance path
        balance(path /*, true*/);
        return true;
    }

    bool remove_aux(DATA_t data)
    {
        Stack<Node *&> path;
        find_path(data, path);
        if (path.isEmpty() || path.back() == nullptr)
        {
            return false;
        }
        // path NOT empty and back is VALID
        Node *curr = path.back();
        assert(curr != nullptr);
        if (curr->isLeaf())
        {
            delete curr;
            path.back() = nullptr;
            path.pop_back();
        }
        else if (curr->hasLeft() && !curr->hasRight())
        {
            path.back() = curr->__left;
            delete curr;
            path.pop_back();
        }
        else if (!curr->hasLeft() && curr->hasRight())
        {
            path.back() = curr->__right;
            delete curr;
            path.pop_back();
        }
        else
        { // (curr->hasLeft() && curr->hasRight())

            Stack<Node *&> path_to_successor;
            // find successor
            Node **successor = &curr->__right; // go right once
            while ((*successor) != nullptr)    // go all the way to the left
            {
                path_to_successor.push_back(*successor);
                successor = &((*successor)->__left);
            }

            if (path_to_successor.isEmpty())
            {
                Node *toDelete = path.back();
                path.back() = (*successor);
                delete toDelete;
            }
            // successor DOESN'T have left by definition
            // else if (path.back() == curr->__right)
            //{
            //    path.back()->__left = curr->__left;
            //    path.back()->updateValues();
            //    path[index_of_pointer_to_middle_node] = path.back();
            //    delete curr;
            //}
            else
            {
                // our::swap(path[index_of_pointer_to_middle_node]->__left,    path.back()->__left);     // switch lefts
                // our::swap(path[index_of_pointer_to_middle_node]->__right,   path.back()->__right);   // switch rights
                // our::swap(path[index_of_pointer_to_middle_node]->__height,  path.back()->__height); // switch heights
                // our::swap(path[index_of_pointer_to_middle_node],            path.back());
                our::swap(path.back()->__data, path_to_successor.back()->__data);
                if (path_to_successor.back()->isLeaf())
                {
                    delete path_to_successor.back();
                    path_to_successor.back() = nullptr;
                    path_to_successor.pop_back();
                }
                else
                {
                    Node *toDelete = path_to_successor.back();
                    path_to_successor.back() = path_to_successor.back()->__right;
                    delete toDelete;
                    path_to_successor.pop_back();
                }
            }
            balance(path_to_successor /*, false */);
        }
        // balance path
        balance(path /*, false */);

        return true;
    }

    template <typename FunctionObject>
    bool in_order_traversal_aux_recursive(Node *const root, FunctionObject do_something) const
    {
        if (root == nullptr)
        {
            return false;
        }
        in_order_traversal_aux_recursive(root->__left, do_something);
        do_something(root->__data);
        in_order_traversal_aux_recursive(root->__right, do_something);
        return true;
    }

    template <typename FunctionObject>
    bool reverse_in_order_traversal_aux_recursive(Node *const root, FunctionObject do_something) const
    {
        if (root == nullptr)
        {
            return false;
        }
        reverse_in_order_traversal_aux_recursive(root->__right, do_something);
        do_something(root->__data);
        reverse_in_order_traversal_aux_recursive(root->__left, do_something);
        return true;
    }

    void display(Node *cur, int depth = 0, int state = 0)
    { // state: 1 -> left, 2 -> right , 0 -> root
        if (cur->__left)
            display(cur->__left, depth + 1, 1);

        for (int i = 0; i < depth; i++)
            printf("     ");

        if (state == 1) // left
            printf("┌───");
        else if (state == 2) // right
            printf("└───");

        std::cout << "[" << cur->__data << ",  " << cur->__height << "]" << std::endl;

        if (cur->__right)
            display(cur->__right, depth + 1, 2);
    }

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
       +---------------------------------------------------------------+
       |                                                               |
       |                    Node Implementations                       |
       |                                                               |
       +---------------------------------------------------------------+
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
};

template <typename DATA_t, Comparison (*compFunction)(const DATA_t &, const DATA_t &)>
AVLTree<DATA_t, compFunction>::AVLTree() : __root(nullptr), __min_element(nullptr), __max_element(nullptr), __size(0)
{
}

template <typename DATA_t, Comparison (*compFunction)(const DATA_t &, const DATA_t &)>
AVLTree<DATA_t, compFunction>::~AVLTree()
{
    clear();
}

template <typename DATA_t, Comparison (*compFunction)(const DATA_t &, const DATA_t &)>
void AVLTree<DATA_t, compFunction>::insert(DATA_t data)
{
    if (insert_aux(data)) // insert successful
    {
        __size++;
        __min_element = findMin();
        __max_element = findMax();
    }
    else
    {
        throw ElementAlreadyExistsException();
    }
}

template <typename DATA_t, Comparison (*compFunction)(const DATA_t &, const DATA_t &)>
void AVLTree<DATA_t, compFunction>::remove(DATA_t data)
{
    if (remove_aux(data)) // deletion successful
    {
        __size--;
        if (isEmpty())
        {
            __min_element = nullptr;
            __max_element = nullptr;
        }
        else
        {
            __min_element = findMin();
            __max_element = findMax();
        }
    }
    else
    {
        throw NoSuchElementException();
    }
}

template <typename DATA_t, Comparison (*compFunction)(const DATA_t &, const DATA_t &)>
void AVLTree<DATA_t, compFunction>::clear()
{
    __size -= clear_aux(__root);
    assert(__size == 0);
    __root = nullptr;
    __min_element = nullptr;
    __max_element = nullptr;
}

template <typename DATA_t, Comparison (*compFunction)(const DATA_t &, const DATA_t &)>
bool AVLTree<DATA_t, compFunction>::isEmpty() const
{
    return (__root == nullptr);
}

template <typename DATA_t, Comparison (*compFunction)(const DATA_t &, const DATA_t &)>
int AVLTree<DATA_t, compFunction>::getSize() const
{
    return __size;
}

template <typename DATA_t, Comparison (*compFunction)(const DATA_t &, const DATA_t &)>
DATA_t &AVLTree<DATA_t, compFunction>::find(const DATA_t &data)
{
    Node *temp = *find_node(data);
    if (temp == nullptr)
    {
        throw NoSuchElementException();
    }
    return temp->__data;
}

template <typename DATA_t, Comparison (*compFunction)(const DATA_t &, const DATA_t &)>
const DATA_t &AVLTree<DATA_t, compFunction>::find(const DATA_t &data) const
{
    return find(data);
}

template <typename DATA_t, Comparison (*compFunction)(const DATA_t &, const DATA_t &)>
const DATA_t &AVLTree<DATA_t, compFunction>::getMin() const
{
    if (__root == nullptr)
    {
        throw NoSuchElementException();
    }
    return findMin()->__data;
}

template <typename DATA_t, Comparison (*compFunction)(const DATA_t &, const DATA_t &)>
const DATA_t &AVLTree<DATA_t, compFunction>::getMax() const
{
    if (__root == nullptr)
    {
        throw NoSuchElementException();
    }
    return findMax()->__data;
}

template <typename DATA_t, Comparison (*compFunction)(const DATA_t &, const DATA_t &)>
void AVLTree<DATA_t, compFunction>::display()
{
    std::cout << "\n";
    if (!isEmpty())
        display(__root);
    else
        std::cout << "Empty";
    std::cout << "\n";
}

#endif // _AVL_TREE_H_
