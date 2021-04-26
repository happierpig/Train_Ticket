#include <iostream>
#include "BPlusTree.hpp"
#include "functions.hpp"
int main(){
    BPlusTree<int,int> myTree("test");
    myTree.createRoot(5,5);
    std::cout << myTree.findLeaf(3);
    return 0;
}