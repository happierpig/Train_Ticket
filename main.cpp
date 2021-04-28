#include <iostream>
#include "BPlusTree.hpp"
#include "functions.hpp"
int main(){
    BPlusTree<int,int,4,3> myTree("test");
    while (true){
        int x;
        std::cin >> x;
        if(x == -1) break;
        myTree.insert(x,x);
        myTree.show();
    }
    return 0;
}