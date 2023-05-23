#ifndef _STACK_H_
#define _STACK_H_

#include <iostream>

template <typename T>
class Node
{
public:
    T data;
    Node<T> *next;
    Node<T> *prev;

    Node(T value, Node<T> *nextNode = nullptr, Node<T> *prevNode = nullptr)
        : data(value), next(nextNode), prev(prevNode) {}
};

template <typename T>
class Stack
{
public:
    Stack() : top(nullptr), bottom(nullptr), _size(0) {}

    ~Stack()
    {
        while (!isEmpty())
        {
            pop_back();
        }
    }

    void push_back(T data)
    {
        Node<T> *newNode = new Node<T>(data);

        if (isEmpty())
        {
            top = bottom = newNode;
        }
        else
        {
            top->next = newNode;
            newNode->prev = top;
            top = newNode;
        }
        _size++;
    }

    void pop_back()
    {
        if (isEmpty())
        {
            throw std::runtime_error("Stack is isEmpty");
        }

        Node<T> *tempNode = top;
        if (top->prev != nullptr)
        {
            top = top->prev;
            top->next = nullptr;
        }
        else
        {
            top = bottom = nullptr;
        }

        if (top != nullptr)
        {
            top->next = nullptr;
        }
        else
        {
            bottom = nullptr;
        }

        delete tempNode;
        _size--;
    }

    T back()
    {
        if (isEmpty())
        {
            throw std::runtime_error("Stack is isEmpty");
        }

        return top->data;
    }

    bool isEmpty()
    {
        return _size == 0;
    }

    int size() const
    {
        return _size;
    }

    T &operator[](int index)
    {
        if (index < 0 || index >= _size)
        {
            throw std::out_of_range("Index out of range");
        }

        Node<T> *currentNode = bottom;
        for (int i = 0; i < index; i++)
        {
            currentNode = currentNode->next;
        }

        return currentNode->data;
    }

    const T &operator[](int index) const
    {
        if (index < 0 || index >= _size)
        {
            throw std::out_of_range("Index out of range");
        }

        Node<T> *currentNode = bottom;
        for (int i = 0; i < index; i++)
        {
            currentNode = currentNode->next;
        }

        return currentNode->data;
    }

private:
    Node<T> *top;
    Node<T> *bottom;
    int _size;
};

#endif // _STACK_H_