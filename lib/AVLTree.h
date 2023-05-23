#ifndef _AVL_TREE_H_
#define _AVL_TREE_H_

#include <string>
#include <iostream>
#include <exception>

#include <vector>
#include <algorithm>

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
private:
    std::vector<T> data;

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



    void insert(T value)
    {
        auto it = data.begin();

        while (it != data.end())
        {
            Comparison result = compFunction(*it, value);
            if (result == Comparison::less)
            {
                ++it;
            }
            else if (result == Comparison::greater)
            {
                break;
            }
            else
            {
                throw ElementAlreadyExistsException();
            }
        }

        data.insert(it, value);
    }
    void remove(T value)
    {
        auto it = data.begin();
        bool found = false;

        while (it != data.end())
        {
            if (compFunction(*it, value) == Comparison::equal) // Equivalent to *it == value
            {
                it = data.erase(it);
                found = true;
                break; // Assuming you only want to remove one instance of the value
            }
            else
            {
                ++it;
            }
        }

        if (!found)
        {
            throw NoSuchElementException();
        }
    }

    void clear()
    {
        data.clear();
    }

    bool isEmpty() const
    {
        return data.empty();
    }

    int getSize() const
    {
        return data.size();
    }

    T &find(T value)
    {
        auto it = data.begin();
        while (it != data.end())
        {
            if (compFunction(*it, value) == Comparison::equal) // Equivalent to *it == value
            {
                return *it;
            }
            else
            {
                ++it;
            }
        }
        throw NoSuchElementException();
    }

    const T &find(T value) const
    {
        return find(value);
    }

    T &getMin()
    {
        if (isEmpty())
        {
            throw NoSuchElementException();
        }
        return data.front();
    }

    T &getMax()
    {
        if (isEmpty())
        {
            throw NoSuchElementException();
        }
        return data.back();
    }

    void display()
    {
        if (isEmpty())
        {
            std::cout << "Empty\n";
            return;
        }
        for (auto elem : data)
        {
            std::cout << elem << " ";
        }
        std::cout << "\n";
    }

    template <typename FunctionObject>
    void in_order_traversal(FunctionObject do_something) 
    {
        for (auto &elem : data)
        {
            do_something(elem);
        }
    }

    template <typename FunctionObject>
    void reverse_in_order_traversal(FunctionObject do_something) 
    {
        for (auto rit = data.rbegin(); rit != data.rend(); ++rit)
        {
            do_something(*rit);
        }
    }
};

#endif // _AVL_TREE_H_
