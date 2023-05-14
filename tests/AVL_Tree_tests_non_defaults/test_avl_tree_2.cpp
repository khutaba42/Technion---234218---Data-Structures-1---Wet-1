#include <iostream>
#include <cassert>
#include "AVLTree.h"


Comparison oppositeCompare(const int& left, const int& right) {
    if (left > right)
        return Comparison::less;
    if (left < right)
        return Comparison::greater;
    return Comparison::equal;
}

int main() {

    AVLTree<int, oppositeCompare> avl;

    // test insertion and size
    assert(avl.getSize() == 0);
    
    assert(avl.insert(10) == true);
    
    assert(avl.getSize() == 1);
    
    assert(avl.insert(20) == true);
    
    assert(avl.getSize() == 2);
    
    assert(avl.insert(30) == true);
    
    assert(avl.getSize() == 3);
    
    assert(avl.insert(40) == true);
    
    assert(avl.getSize() == 4);
    
    assert(avl.insert(50) == true);
    
    assert(avl.getSize() == 5);
    
    assert(avl.insert(60) == true);
    
    assert(avl.getSize() == 6);
    
    assert(avl.insert(70) == true);
    
    assert(avl.getSize() == 7);
    
    assert(avl.insert(80) == true);
    
    assert(avl.getSize() == 8);
    
    assert(avl.insert(90) == true);
    
    assert(avl.getSize() == 9);
    
    assert(avl.insert(100) == true);
    
    assert(avl.getSize() == 10);
    

    // test duplicate insertion
    assert(avl.insert(20) == false);
    assert(avl.getSize() == 10);

    // test search and removal
    assert(avl.remove(50) == true);
    avl.print();
    assert(avl.getSize() == 9);
    avl.print();
    assert(avl.remove(50) == false);
    avl.print();
    assert(avl.getSize() == 9);
    avl.print();
    assert(avl.remove(30) == true);
    avl.print();
    assert(avl.getSize() == 8);
    avl.print();
    assert(avl.remove(10) == true);
    avl.print();
    assert(avl.getSize() == 7);
    avl.print();
    assert(avl.remove(80) == true);
    avl.print();
    assert(avl.getSize() == 6);
    avl.print();
    assert(avl.remove(90) == true);
    avl.print();
    assert(avl.getSize() == 5);
    avl.print();
    assert(avl.remove(60) == true);
    avl.print();
    assert(avl.getSize() == 4);
    avl.print();
    assert(avl.remove(40) == true);
    avl.print();
    assert(avl.getSize() == 3);
    avl.print();
    assert(avl.remove(20) == true);
    avl.print();
    assert(avl.getSize() == 2);
    avl.print();
    assert(avl.remove(100) == true);
    avl.print();
    assert(avl.getSize() == 1);
    avl.print();
    assert(avl.remove(70) == true);
    avl.print();
    assert(avl.getSize() == 0);
    avl.print();
    assert(avl.isEmpty() == true);
    avl.print();
    


    // test clear and isEmpty
    assert(avl.insert(10) == true);
    assert(avl.insert(20) == true);
    assert(avl.getSize() == 2);
    assert(avl.isEmpty() == false);
    avl.clear();
    assert(avl.getSize() == 0);
    assert(avl.isEmpty() == true);

    // test height
    assert(avl.insert(10) == true);
    avl.print();
    assert(avl.insert(20) == true);
    avl.print();
    assert(avl.insert(30) == true);
    avl.print();
    assert(avl.insert(40) == true);
    avl.print();
    assert(avl.insert(50) == true);
    avl.print();
    assert(avl.insert(60) == true);
    avl.print();
    assert(avl.insert(70) == true);
    avl.print();
    assert(avl.insert(80) == true);
    avl.print();
    assert(avl.insert(90) == true);
    avl.print();
    assert(avl.insert(100) == true);
    avl.print();
    std::cout << avl.height() << std::endl;
    assert(avl.height() == 3);
    avl.print();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
