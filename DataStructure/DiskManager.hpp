//
// Created by 赵先生 on 2021/4/8.
//

#ifndef TRAIN_TICKET_DISKMANAGER_HPP
#define TRAIN_TICKET_DISKMANAGER_HPP
//#define debug
#include <cstring>
#include <fstream>
#include "HashMap.hpp"

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
    class List{
    public:
        struct Node{
            Node * before;
            Node * after;
            T * data;
            int position;
            bool isWriten = false;
            Node() = delete;
            Node(Node * _bef,Node * _aft,const T & _data,int _pos):before(_bef),after(_aft),data(new T(_data)),position(_pos),isWriten(false){}
            Node(Node * _bef,Node * _aft):before(_bef),after(_aft),data(nullptr),position(-1),isWriten(false){}
            ~Node(){
                delete data;
            }
        };
        int capacity;
        Node * head;Node * tail;
        int dataSize;
        DiskManager * theDisk;
    private:
        void write_back(int _pos,T * _data){
            theDisk->file.seekp(_pos,ios::beg);
            theDisk->file.write(reinterpret_cast<const char *>(_data),sizeof(*_data));
        }
        void pop_back(){
            Node * tmp = tail->before;
            if(tmp->isWriten) this->write_back(tmp->position,tmp->data);
            tmp->before->after = tmp->after;
            tmp->after->before = tmp->before;
            theDisk->assistantMap.erase(tmp->position);
            --dataSize;
            delete tmp;
        }
        Node * push_front(int _pos,const T & _data){
            Node * tmp = new Node(head,head->after,_data,_pos);
            tmp->after->before = tmp;
            head->after = tmp;
            ++dataSize;
            if(dataSize == capacity) pop_back();
            return tmp;
        }
    public:
        List() = delete;
        explicit List(int _capacity,DiskManager * _ptr):theDisk(_ptr),capacity(_capacity),dataSize(0),head(new Node(nullptr, nullptr)),tail(new Node(nullptr, nullptr)){
            head->after = tail;
            tail->before = head;
        }
        ~List(){
            Node * tmp = head;
            theDisk->file.open(theDisk->fileName,ios::in | ios::out | ios::binary);
            while(tmp != nullptr){
                if(tmp->position != -1){
                    if(tmp->isWriten) this->write_back(tmp->position,tmp->data);
                }
                Node * tmpp = tmp;
                tmp = tmp->after;
                delete tmpp;
            }
        }
        Node * insert(int _pos,const T & _data){
            return this->push_front(_pos,_data);
        }
        void update(Node * _target){
//            _target->isWriten = true;
            if(head->after == _target) return;
            _target->before->after = _target->after;
            _target->after->before = _target->before;
            _target->before = head;
            _target->after = head->after;
            head->after->before = _target;
            head->after = _target;
        }
        void erase(Node * _target){
            --dataSize;
            _target->before->after = _target->after;
            _target->after->before = _target->before;
            delete _target;
        }
        void clear(){
            Node * tmp = head;
            while(tmp != nullptr){
                Node * tmpp = tmp;
                tmp = tmp->after;
                delete tmpp;
            }
            dataSize = 0;
            head = new Node(nullptr, nullptr);
            tail = new Node(nullptr, nullptr);
            head->after = tail;
            tail->before = head;
        }

#ifdef debug
        void show(){
            std::cout << theDisk->fileName <<" Cache List show : ------" << std::endl;
            Node * tmp = head;
            while(tmp != nullptr){
                if(tmp->position > 0) tmp->data->show();
                tmp = tmp->after;
            }
            std::cout << "---- over -----" << std::endl;
        }
#endif
    };
    List cache;
    HashMap<int,typename List::Node *> assistantMap;
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
            file.write(reinterpret_cast<const char *>(&nowPtr),sizeof(nowPtr));
            nowPtr = index;
            nextPtr = -1;
            return;
        }else{
            nextPtr = nowPtr;
            file.seekp(index,ios::beg);
            file.write(reinterpret_cast<const char *>(&nowPtr),sizeof(nowPtr));
            nowPtr = index;
            return;
        }
    }
public:
    DiskManager() = delete;
    explicit DiskManager(const string & _name,int _capacity):fileName(_name),cache(_capacity,this),assistantMap(_capacity){
        file.open(fileName,ios::in);
        if(file.fail()){
            file.clear();
            file.open(fileName,ios::out);
            file.close();
            file.open(fileName,ios::out | ios::in | ios::binary);
            nowPtr = -1;nextPtr = -1;
            basicInfo temp;
            file.seekp(0,ios::beg);
            file.write(reinterpret_cast<const char *>(&temp),sizeof(temp));
            file.seekp(sizeof(temp),ios::beg);
            file.write(reinterpret_cast<const char *>(&nowPtr),sizeof(nowPtr));
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
        file.write(reinterpret_cast<const char *>(&nowPtr),sizeof(nowPtr));
        file.close();
    }

public:
    int write(const T & data){
        int temp;
        if(nowPtr < 0){
            file.seekp(0,ios::end);
            temp = file.tellp();
            file.write(reinterpret_cast<const char *>(&data),sizeof(data));
            return temp;
        }else{
            temp = nowPtr;
            file.seekp(temp,ios::beg);
            file.write(reinterpret_cast<const char *>(&data),sizeof(data));
            setSparePointer();
            return temp;
        }
    }
    void write(const T * data,int position){
        if(assistantMap.exist(position)){
            assistantMap.find(position)->isWriten = true;
        }else {
            if (file.fail()) {
                file.clear();
                file.close();
                file.open(fileName, ios::in | ios::out | ios::binary);
            }
            file.seekp(position, ios::beg);
            file.write(reinterpret_cast<const char *>(data), sizeof(*data));
        }
    }
    T * read(int index,bool mode = false) {
        if(assistantMap.exist(index)){
            typename List::Node * tmp = assistantMap.find(index);
            cache.update(tmp);
            if(mode) tmp->isWriten = true;
            return tmp->data;
        }else {
            T temp;
            file.seekp(index, ios::beg);
            file.read(reinterpret_cast< char *>(&temp), sizeof(temp));
            typename List::Node * tmp = cache.insert(index,temp);
            assistantMap.insert(index,tmp);
            if(mode) tmp->isWriten = true;
            return tmp->data;
        }
    }

    void erase(int index){
        if(assistantMap.exist(index)){
            cache.erase(assistantMap.find(index));
            assistantMap.erase(index);
        }
        addSpare(index);
    }

    basicInfo tellInfo(){
        basicInfo temp;
        file.seekp(0,ios::beg);
        file.read(reinterpret_cast< char *>(&temp),sizeof(temp));
        return temp;
    }
    int tellp(){
        if(file.fail()){
            file.clear();
            file.close();
            file.open(fileName,ios::out | ios::in | ios::binary);
        }
        if(nowPtr >= 0) return nowPtr;
        else{
            file.seekp(0,ios::end);
            return file.tellp();
        }
    }
    void setInfo(const basicInfo & info){
        file.seekp(0,ios::beg);
        file.write(reinterpret_cast<const char *>(&info),sizeof(info));
    }
    void clear(){
        file.close();
        file.open(fileName,ios::out);
        file.close();
        file.open(fileName,ios::in | ios::out);
        nowPtr = -1;nextPtr = -1;
        basicInfo temp;
        file.seekp(0,ios::beg);
        file.write(reinterpret_cast<const char *>(&temp),sizeof(temp));
        file.seekp(sizeof(temp),ios::beg);
        file.write(reinterpret_cast<const char *>(&nowPtr),sizeof(nowPtr));
        assistantMap.clear();
        cache.clear();
    }
#ifdef debug
    void show(){
        cache.show();
    }
#endif

};
#endif //TRAIN_TICKET_DISKMANAGER_HPP
