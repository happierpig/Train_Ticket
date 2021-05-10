#include <iostream>
#include "DataStructure/HashMap.hpp"
using std::cin;
int main(){
    HashMap<int,int> test(131);
    while(true){
        int opClass;
        cin >> opClass;
        if(opClass == -1) break;
        if(opClass == 1){
            int x,y;cin >> x >> y;
            if(test.exist(x)) std::cerr << "already exists" << std::endl;
            else{
                test.insert(x,y);
            }
        }
        if(opClass == 2){
            int x;cin >> x;
            test.erase(x);
        }
        if(opClass == 3){
            int x;cin >>x;
            if(!test.exist(x)) std::cerr << "doesn't exist" << std::endl;
            else std::cout << test.find(x) << std::endl;
        }
    }
    return 0;
}