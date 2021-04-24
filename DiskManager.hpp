//
// Created by 赵先生 on 2021/4/8.
//

#ifndef TRAIN_TICKET_DISKMANAGER_HPP
#define TRAIN_TICKET_DISKMANAGER_HPP

#include <cstring>
#include <fstream>
using std::string;
using std::fstream;
using std::ios;

template <class T,class basicInfo>
class DiskManager{
private:
    const string fileName;
    int nowPtr,nextPtr; // nowPtr is the head of the fileLinkList
    fstream file;
private:
    void setSparePointer(){ // use this function after using nowPtr
        if(nowPtr < 0) return;
        nowPtr = nextPtr;
        if(nextPtr > 0){
            file.seekp(nextPtr,ios::beg);
            file.read(reinterpret_cast<char * >(&nextPtr),sizeof(nextPtr));
        }
    }
    void addSpare(int index){
        if(nowPtr < 0){
            file.seekp(index,ios::beg);
            file.write(reinterpret_cast<char *>(&nowPtr),sizeof(nowPtr));
            nowPtr = index;
            nextPtr = -1;
            return;
        }else{
            nextPtr = nowPtr;
            file.seekp(index,ios::beg);
            file.write(reinterpret_cast<char *>(&nowPtr),sizeof(nowPtr));
            nowPtr = index;
            return;
        }
    }
public:
    explicit DiskManager(string & _name):fileName(_name){
        file.open(fileName,ios::in);
        if(file.fail()){
            file.clear();
            file.open(fileName,ios::in | ios::out);
            nowPtr = -1;nextPtr = -1;
            basicInfo temp;
            file.seekp(0,ios::beg);
            file.write(reinterpret_cast<char *>(&temp),sizeof(temp));
            file.seekp(sizeof(temp),ios::beg);
            file.write(reinterpret_cast<char *>(&nowPtr),sizeof(nowPtr));
        }else{
            file.close();
            file.open(fileName,ios::in | ios::out | ios::binary);
            file.seekp(sizeof(basicInfo),ios::beg);
            file.read(reinterpret_cast<char *>(&nowPtr),sizeof(nowPtr));
            if(nowPtr > 0){
                file.seekp(nowPtr,ios::beg);
                file.read(reinterpret_cast<char *>(&nextPtr),sizeof(nextPtr));
            }else nextPtr = -1;
        }
    }
    ~DiskManager(){
        file.seekp(sizeof(basicInfo),ios::beg);
        file.write(reinterpret_cast<char *>(&nowPtr),sizeof(nowPtr));
        file.close();
    }

public:
    int write(const T & data){
        int temp;
        if(nowPtr < 0){
            file.seekp(0,ios::end);
            temp = file.tellp();
            file.write(reinterpret_cast<char *>(&data),sizeof(data));
            return temp;
        }else{
            temp = nowPtr;
            file.seekp(temp,ios::beg);
            file.write(reinterpret_cast<char *>(&data),sizeof(data));
            setSparePointer();
            return temp;
        }
    }

    T read(int index){
        T temp;
        file.seekp(index,ios::beg);
        file.read(reinterpret_cast<char *>(&temp),sizeof(temp));
        return temp;
    }

    void erase(int index){
        addSpare(index);
    }

    basicInfo tellInfo(){
        basicInfo temp;
        file.seekp(0,ios::beg);
        file.read(reinterpret_cast<char *>(&temp),sizeof(temp));
        return temp;
    }

    void setInfo(const basicInfo & info){
        file.seekp(0,ios::beg);
        file.write(reinterpret_cast<char *>(&info),sizeof(info));
    }
};
#endif //TRAIN_TICKET_DISKMANAGER_HPP
