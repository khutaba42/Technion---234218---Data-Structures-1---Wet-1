#include <iostream>
#include <cassert>
#include "AVLTree.h"

static void DO_SOME(const int x) {
    std::cout << x << " $\n" ;
}

int main()
{

    AVLTree<int> avl;

    // test insertion and size
    assert(avl.insert(10) == true);
    assert(avl.insert(20) == true);
    assert(avl.insert(30) == true);
    assert(avl.insert(40) == true);
    assert(avl.insert(50) == true);
    assert(avl.insert(60) == true);
    assert(avl.insert(70) == true);
    assert(avl.insert(80) == true);
    assert(avl.insert(90) == true);
    assert(avl.insert(100) == true);


    avl.in_order_traversal(DO_SOME);



    std::cout << "All tests passed!" << std::endl;
    return 0;
}
