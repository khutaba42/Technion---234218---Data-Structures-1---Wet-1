#include <iostream>
#include <cassert>
#include "AVLTree.h"

Comparison stringCompare(const std::string& left, const std::string& right) {
    if (left.length() < right.length())
        return Comparison::less;
    if (left.length() > right.length())
        return Comparison::greater;
    return Comparison::equal;
}
int main() {
    AVLTree<std::string,
            stringCompare
    > avl;

    // Test insertion and size
    assert(avl.getSize() == 0);

    assert(avl.insert("one") == true);
    assert(avl.getSize() == 1);

    assert(avl.insert("twoaaaa") == true);
    assert(avl.getSize() == 2);

    assert(avl.insert("three") == true);
    assert(avl.getSize() == 3);

    // Test duplicate insertion
    assert(avl.insert("twoaaaa") == false);
    assert(avl.getSize() == 3);

    // Test search and removal
    assert(avl.remove("twoaaaa") == true);
    assert(avl.getSize() == 2);
    assert(avl.remove("twoaaaa") == false);
    assert(avl.getSize() == 2);
    assert(avl.remove("three") == true);
    assert(avl.getSize() == 1);

    // Test clear and isEmpty
    assert(avl.insert("four") == true);
    assert(avl.insert("fiveeeeee") == true);
    assert(avl.getSize() == 3);
    assert(avl.isEmpty() == false);
    avl.clear();
    assert(avl.getSize() == 0);
    assert(avl.isEmpty() == true);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
