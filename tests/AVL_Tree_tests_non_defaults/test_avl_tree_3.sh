# write atester.sh in the terminal to run the test

for i in {1..20}
do
  echo "\n"
done

g++ -g -I./lib -std=c++11 tests/AVL_Tree_tests_non_defaults/test_avl_tree_3.cpp -o tests/AVL_Tree_tests_non_defaults/test_avl_tree_3.out

tests/AVL_Tree_tests_non_defaults/test_avl_tree_3.out

rm tests/AVL_Tree_tests_non_defaults/test_avl_tree_3.out

for i in {1..2}
do
  echo "\n"
done