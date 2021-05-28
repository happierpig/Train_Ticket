//
// Created by 赵先生 on 2021/5/28.
//

#ifndef CODE_DISKSTACK_HPP
#define CODE_DISKSTACK_HPP
#include <fstream>
#include <cstring>
using std::string;
using std::fstream;
using std::ios;

template <int capacity = 1000>
class DiskStack{
private:
    string fileName;
    fstream file;
    int dataSet[capacity];
    int top;
public:
    explicit DiskStack(const string & _name):fileName(_name + "_diskStack.dat"){
        file.open(fileName,ios::in);
        memset(dataSet,0,sizeof(dataSet));
        if(file.fail()){
            file.clear();
            file.open(fileName,ios::out);
            file.close();
            file.open(fileName,ios::out | ios::in | ios::binary);
            file.seekp(0,ios::beg);
            this->top = 0;
            file.write(reinterpret_cast<const char *>(&top),sizeof(top));
        }else{
            file.close();
            file.open(fileName,ios::in | ios::out | ios::binary);
            file.seekp(0,ios::beg);
            file.read(reinterpret_cast<char *>(&top),sizeof(top));
            if(top > 0) {
                file.seekp(sizeof(top), ios::beg);
                file.read(reinterpret_cast<char *>(dataSet), sizeof(dataSet));
            }
        }
    }
    ~DiskStack(){
        file.seekp(0,ios::beg);
        file.write(reinterpret_cast<const char *>(&top),sizeof(top));
        if(top > 0){
            file.seekp(sizeof(top), ios::beg);
            file.write(reinterpret_cast<const char *>(dataSet), sizeof(dataSet));
        }
    }
    int getPos(){
        if(top == 0) return -1;
        else{
            int tmp = dataSet[top--];
            if(tmp <= 0) throw 1;
            return tmp;
        }
    }
    void savePos(int pos){
        dataSet[++top] = pos;
        if(top >= capacity) throw 2;
    }
    void
};


#endif //CODE_DISKSTACK_HPP
