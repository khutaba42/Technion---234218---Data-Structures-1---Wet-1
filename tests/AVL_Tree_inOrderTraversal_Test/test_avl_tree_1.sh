# write atester.sh in the terminal to run the test

for i in {1..2}
do
  echo "\n"
done

g++ -g -I./lib -std=c++11 tests/AVL_Tree_inOrderTraversal_Test/test_avl_tree_1.cpp -o tests/AVL_Tree_inOrderTraversal_Test/test_avl_tree_1.out

tests/AVL_Tree_inOrderTraversal_Test/test_avl_tree_1.out

rm tests/AVL_Tree_inOrderTraversal_Test/test_avl_tree_1.out

for i in {1..2}
do
  echo "\n"
done