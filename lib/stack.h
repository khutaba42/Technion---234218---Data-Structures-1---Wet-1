#ifndef _AVL_TREE_STACK_H_
#define _AVL_TREE_STACK_H_

#include <iostream>

template <typename T>
class Stack
{
public:
    Stack();

    ~Stack();

    void push_back(T data);

    void pop_back();

    T back();

    bool isEmpty();

    int size() const;

    T &operator[](int index);

    const T &operator[](int index) const;

private:
    struct Node
    {
        T __data;
        Node *__next;
        Node *__prev;

        Node(T value, Node *nextNode = nullptr, Node *prevNode = nullptr)
            : __data(value), __next(nextNode), __prev(prevNode) {}
    };

    Node *__top;
    Node *__bottom;
    int __size;
};

template <typename T>
Stack<T>::Stack() : __top(nullptr), __bottom(nullptr), __size(0) {}

template <typename T>
Stack<T>::~Stack()
{
    while (!isEmpty())
    {
        pop_back();
    }
}

template <typename T>
void Stack<T>::push_back(T data)
{
    Node *newNode = new Node(data);

    if (isEmpty())
    {
        __top = __bottom = newNode;
    }
    else
    {
        __top->__next = newNode;
        newNode->__prev = __top;
        __top = newNode;
    }
    __size++;
}

template <typename T>
void Stack<T>::pop_back()
{
    if (isEmpty())
    {
        throw std::runtime_error("Stack is isEmpty");
    }

    Node *tempNode = __top;
    if (__top->__prev != nullptr)
    {
        __top = __top->__prev;
        __top->__next = nullptr;
    }
    else
    {
        __top = __bottom = nullptr;
    }

    if (__top != nullptr)
    {
        __top->__next = nullptr;
    }
    else
    {
        __bottom = nullptr;
    }

    delete tempNode;
    __size--;
}

template <typename T>
T Stack<T>::back()
{
    if (isEmpty())
    {
        throw std::runtime_error("Stack is isEmpty");
    }

    return __top->__data;
}

template <typename T>
bool Stack<T>::isEmpty()
{
    return __size == 0;
}

template <typename T>
int Stack<T>::size() const
{
    return __size;
}

template <typename T>
T &Stack<T>::operator[](int index)
{
    if (index < 0 || index >= __size)
    {
        throw std::out_of_range("Index out of range");
    }

    Node *currentNode = __bottom;
    for (int i = 0; i < index; i++)
    {
        currentNode = currentNode->__next;
    }

    return currentNode->__data;
}

template <typename T>
const T &Stack<T>::operator[](int index) const
{
    if (index < 0 || index >= __size)
    {
        throw std::out_of_range("Index out of range");
    }

    Node *currentNode = __bottom;
    for (int i = 0; i < index; i++)
    {
        currentNode = currentNode->__next;
    }

    return currentNode->__data;
}

#endif // _AVL_TREE_STACK_H_
