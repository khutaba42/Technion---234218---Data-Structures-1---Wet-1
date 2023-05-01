# write atester.sh in the terminal to run the test

for i in {1..20}
do
  echo "\n"
done

g++ -g -I./lib -std=c++11 test_avl_tree.cpp -o test_avl_tree.out
./test_avl_tree.out