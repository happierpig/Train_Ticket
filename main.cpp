#include <iostream>
#include "Myexception.hpp"
void getNext(char * p)
{
    int next[100] = {0};
    next[0] = -1;
    int i = 0, j = -1;

    while (i < strlen(p))
    {
        if (j == -1 || p[i] == p[j])
        {
            ++i;
            ++j;
            next[i] = j;
        }
        else
            j = next[j];
    }
}
int main(){
//    char  * ch = "ababbba";
//    getNext(ch);
    try{
        error("sos");
    }catch (exception & x){
        std::cout << x.what();
    }
    return 0;
}