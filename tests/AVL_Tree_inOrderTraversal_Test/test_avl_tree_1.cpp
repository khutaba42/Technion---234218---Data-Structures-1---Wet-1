#include <iostream>
#include <cassert>
#include "AVLTree.h"

static void DO_SOME(const int x)
{
    std::cout << x << " $\n";
}

int main()
{

    AVLTree<int> avl;

    // test insertion and size
    std::cout << "test inserion and size!\n" << std::endl;
    for (size_t i = 1; i <= 10; i++)
    {
        std::cout << "insert " << 10*i << " , size="<< i<< std::endl;
        try
        {
            avl.insert(10*i);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
        assert(avl.getSize() == i);
    };

    avl.print();

    std::cout << "test duplicates!" << std::endl;
    for (size_t i = 1; i <= 10; i++)
    {
        std::cout << "insert " << 10*i << " again   -   ";
        try
        {
            avl.insert(10*i);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
        std::cout << "size="<< avl.getSize() << std::endl;
        assert(avl.getSize() == 10);
    };

    std::cout << "traverse the tree!" << std::endl;   
    avl.in_order_traversal(DO_SOME);

    std::cout << "reverse traverse the tree!" << std::endl;   
    avl.reverse_in_order_traversal(DO_SOME);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
