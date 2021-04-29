#include <iostream>
#include "BPlusTree.hpp"
#include <vector>
using std::vector;
int main(){
    BPlusTree<int,int,200,200> myTree("test");
//    myTree.show();
    int test = 0;
    int bullet[100000] = {0};
    srand(time(NULL));
    while (++test < 100000){
        int x = rand() % 90000;
        bullet[x]++;
        myTree.insert(x,x);
        vector<int> vec_ans;
        myTree.find(x,vec_ans);
        if(bullet[x] == vec_ans.size()){
            std::cout << "Test : " << test << " Bingo!" << "  number : " << x << "  size : " << bullet[x] <<std::endl;
        }
        else std::cout << "Test : " << test << " Failed!" << std::endl;
    }
    return 0;
}