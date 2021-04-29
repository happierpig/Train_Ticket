#include <iostream>
#include "BPlusTree.hpp"
#include <vector>
using std::vector;
int main(){
    BPlusTree<int,int,4,3> myTree("test");
    while (true){
        int x;
        std::cin >> x;
        if(x == -1) break;
        if(x == 1) {
            int value;std::cin >> value;
            myTree.insert(value, value);
            myTree.show();
        }
        if(x == 2){
            int target;
            std::cin >> target;
            vector<int> vec_ans;
            myTree.find(target,vec_ans);
            for(int i = 0;i < vec_ans.size();++i) std::cout << vec_ans[i] << ' ';
            std::cout << std::endl;
        }
    }
    return 0;
}