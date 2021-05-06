#include <iostream>
#include "BPlusTree.hpp"
#include <vector>
using std::vector;
int main(){
//    BPlusTree<int,int,200,200> myTree("test");
//    myTree.show();
//    int test = 0;
//    int bullet[100000] = {0};
//    srand(time(NULL));
//    while (++test < 100000){
//        int x = rand() % 90000;
//        bullet[x]++;
//        myTree.insert(x,x);
//        vector<int> vec_ans;
//        myTree.find(x,vec_ans);
//        if(bullet[x] == vec_ans.size()){
//            std::cout << "Test : " << test << " Bingo!" << "  number : " << x << "  size : " << bullet[x] <<std::endl;
//        }
//        else std::cout << "Test : " << test << " Failed!" << std::endl;
//    }
    BPlusTree<int,int,4,4> myTree("test");
    while(true){
        int opClass;
        std::cin >> opClass;
        if(opClass == -1){
            break;
        }
        if(opClass == 1){
            int key,value;
            std::cin >> key >> value;
            myTree.insert(key,value);
            myTree.show();
            continue;
        }
        if(opClass == 2){
            int key,value;
            std::cin >> key >> value;
            myTree.erase(key,value);
            myTree.show();
            continue;
        }
    }
    return 0;
}