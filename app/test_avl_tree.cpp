// #define CATCH_CONFIG_MAIN
// #include <catch2/catch.hpp>
#include "ra/avl_tree.hpp"

//TEST_CASE("basic avl tree rotations test"){
int main(){
    std::cout << "test insertion\n";

    avl_tree<std::pair<int, int>, std::less<int>> t1;
    t1.insert({2,1});
    t1.insert({3,1});
    t1.insert({4,1});

    //test right right
    std::cout << "test right right, should be 3 2 4\n";
    t1.pre_order();
    std::cout << "\n";
    std::cout << "parent preorder\n";
    t1.pre_order_with_parent();
    std::cout << "\n";
    std::cout << "start: " << t1.start()->data_.first << "\n";

    avl_tree<std::pair<int, int>, std::less<int>> t2;
    t2.insert({1,2});
    t2.insert({3,4});
    t2.insert({2,5});

    //test right left
    std::cout << "test right left should be 2 1 3\n";
    t2.pre_order();
    std::cout << "\n";
    std::cout << "parent preorder\n";
    t2.pre_order_with_parent();
    std::cout << "\n";
    std::cout << "start: " << t2.start()->data_.first << "\n";

    avl_tree<std::pair<int, int>, std::less<int>> t3;
    t3.insert({3,1});
    t3.insert({2,1});
    t3.insert({1,4});

    //test left left
    std::cout << "test left left should be 2 1 3\n";
    t3.pre_order();
    std::cout << "\n";
    std::cout << "parent preorder\n";
    t3.pre_order_with_parent();
    std::cout << "\n";
    std::cout << "start: " << t3.start()->data_.first << "\n";

    avl_tree<std::pair<int, int>, std::less<int>> t4;
    t4.insert({4,1});
    t4.insert({2,3});
    t4.insert({3,4});

    //test left right
    std::cout << "test left right should be 3 2 4\n";
    t4.pre_order();
    std::cout << "\n";
    std::cout << "parent preorder\n";
    t4.pre_order_with_parent();
    std::cout << "\n";
    std::cout << "start: " << t4.start()->data_.first << "\n";

    avl_tree<std::pair<int, int>, std::less<int>> t5;
    t5.insert({13,1});
    t5.insert({10,2});
    t5.insert({15,3});
    t5.insert({5,6});
    t5.insert({11,1});
    t5.insert({16,17});
    t5.insert({4,5});
    t5.insert({8,9});
    t5.insert({3,5});

    std::cout << "test large tree, 13 5 4 3 10 8 11 15 16\n";
    t5.pre_order();
    std::cout << "\n";
    std::cout << "parent preorder\n";
    t5.pre_order_with_parent();
    std::cout << "\n";
    std::cout << "start: " << t5.start()->data_.first << "\n";

    avl_tree<std::pair<int, int>, std::less<int>> t6;
    t6.insert({1,2});
    t6.insert({2,3});
    t6.insert({3,4});
    t6.insert({1,4});

    std::cout << "test inserting same element twice, 2 1 3\n";
    t6.pre_order();
    std::cout << "\n";
    std::cout << "parent preorder\n";
    t6.pre_order_with_parent();
    std::cout << "\n";
    std::cout << "start: " << t6.start()->data_.first << "\n";

    std::cout << "test deletion\n";
    avl_tree<std::pair<int, int>, std::less<int>> t7;
    t7.insert({2,1});
    t7.insert({1,2});
    t7.insert({3,4});

    std::cout << "initial tree\n";
    t7.pre_order();
    std::cout << "\n";
    std::cout << "parent preorder\n";
    t7.pre_order_with_parent();
    std::cout << "\n";
    std::cout << "start: " << t7.start()->data_.first << "\n";

    t7.delete_node({3,1});
    std::cout << "deleting node 3\n";
    t7.pre_order();
    std::cout << "\n";
    std::cout << "parent preorder\n";
    t7.pre_order_with_parent();
    std::cout << "\n";
    std::cout << "start: " << t7.start()->data_.first << "\n";

    t7.delete_node({2,1});
    std::cout << "deleting node 2\n";
    t7.pre_order();
    std::cout << "\n";
    std::cout << "parent preorder\n";
    t7.pre_order_with_parent();
    std::cout << "\n";
    std::cout << "start: " << t7.start()->data_.first << "\n";

    t7.delete_node({1,2});
    std::cout << "deleting node 1\n";
    t7.pre_order();
    std::cout << "\n";
    std::cout << "parent preorder\n";
    t7.pre_order_with_parent();
    std::cout << "\n";
    std::cout << "start: " << (t7.start()==nullptr) << " end: " << (t7.end()==nullptr) << "\n";

    t7.delete_node({1,2});
    std::cout << "attempting to delete node 1\n";
    t7.pre_order();
    std::cout << "\n";
    std::cout << "parent preorder\n";
    t7.pre_order_with_parent();
    std::cout << "\n";
    std::cout << "start: " << (t7.start()==nullptr) << " end: " << (t7.end()==nullptr) << "\n";

    std::cout << "test deletion part 2\n";
    avl_tree<std::pair<int, int>, std::less<int>> t8;
    t8.insert({2,1});
    t8.insert({1,2});
    t8.insert({6,6});
    t8.insert({7,8});
    t8.pre_order();
    std::cout << "\n";
    std::cout << "parent preorder\n";
    t8.pre_order_with_parent();
    std::cout << "\n";
    std::cout << "start: " << t8.start()->data_.first << "\n";

    t8.delete_node({6,1});
    std::cout << "deleting node 6\n";
    t8.pre_order();
    std::cout << "\n";
    std::cout << "parent preorder\n";
    t8.pre_order_with_parent();
    std::cout << "\n";
    std::cout << "start: " << t8.start()->data_.first << "\n";

    t8.insert({5,5});
    std::cout << "inserting node 5\n";
    t8.pre_order();
    std::cout << "\n";
    std::cout << "parent preorder\n";
    t8.pre_order_with_parent();
    std::cout << "\n";
    std::cout << "start: " << t8.start()->data_.first << "\n";

    t8.delete_node({7,8});
    std::cout << "deleting node 7\n";
    t8.pre_order();
    std::cout << "\n";
    std::cout << "parent preorder\n";
    t8.pre_order_with_parent();
    std::cout << "\n";
    std::cout << "start: " << t8.start()->data_.first << "\n";

    std::cout << "testing larger delete case\n";
    avl_tree<std::pair<int, int>, std::less<int>> t9;
    t9.insert({2,1});
    t9.insert({1,2});
    t9.insert({6,7});
    t9.insert({7,8});
    t9.insert({8,9});
    t9.insert({12,9});
    t9.insert({66,9});
    t9.insert({17,0});
    t9.pre_order();
    std::cout << "\n";
    std::cout << "parent preorder\n";
    t9.pre_order_with_parent();
    std::cout << "\n";
    std::cout << "start: " << t9.start()->data_.first << "\n";

    t9.delete_node({7,9});
    std::cout << "deleting node 7, should be 8 2 1 6 17 12 66\n";
    t9.pre_order();
    std::cout << "\n";
    std::cout << "parent preorder\n";
    t9.pre_order_with_parent();
    std::cout << "\n";
    std::cout << "start: " << t9.start()->data_.first << "\n";

    std::cout << "testing search\n";
    std::cout << "should print key 8: " << t9.search({8,2})->data_.first << "\n";
    std::cout << "should print key 2: " << t9.search({8,3})->data_.first << "\n";
    std::cout << "should print key 66: " << t9.search({66,7})->data_.first << "\n";
    std::cout << "should print 1: " << (t9.search({400,1}) == nullptr) << "\n";

    node<std::pair<int, int>>* cur = t9.start();

    for(int i = 0; i < 7; i++){
        std::cout << cur->data_.first << "\n";
        cur = t9.successor(cur);
    }

    cur = t9.search({66, 7});
    for(int i = 0; i < 7; i++){
        std::cout << cur->data_.first << "\n";
        cur = t9.predecessor(cur);
    }

    avl_tree<std::pair<std::string, int>, std::less<std::string>> t10;
    t10.insert({"hello", 3});
}