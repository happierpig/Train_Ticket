//
// Created by 赵先生 on 2021/4/8.
//

#ifndef TRAIN_TICKET_BPLUSTREE_HPP
#define TRAIN_TICKET_BPLUSTREE_HPP
#include "DiskManager.hpp"
#include "Myexception.hpp"
#include <cstring>
#include <vector>

using std::string;
using std::vector;

template <class Key,class Data,int M = 200,int L = 200>
class BPlusTree{
private:
    class basicInfo{
        int root = -1; //the position of rootNode
        int head = -1; //the head of the leafNode List
        int size = 0; // size of the element
    };
    // below is the parameters of the tree
    static constexpr int MAX_CHILD = M;
    static constexpr int MIN_CHILD = M / 2;
    static constexpr int MAX_RECORD = L;
    static constexpr int MIN_RECORD = L / 2;
    // below is the definition of the node
    class leafNode{
    public:
        int position = -1; // position in leafDisk
        int father = -1;
        int leftBrother = -1;
        int rightBrother = -1;
        int dataSize = 0;
        Key dataKey[MAX_RECORD];
        int dataPosition[MAX_RECORD]; // position in dataDisk
        
    };
    class Node{
    public:
        int position = -1; // position in nodeDisk
        int father = -1;
        int leftBrother = -1;
        int rightBrother = -1;
        int childSize = 0;
        Key nodeKey[MAX_CHILD];
        int childPosition[MAX_CHILD]; // position in nodeDisk(when child is leaf,position is in leafDisk)
        bool childIsLeaf = false;

    };
private:
    //below are private members of the tree
    basicInfo treeInfo;
    DiskManager<leafNode,basicInfo> leafDisk;
    DiskManager<Node,basicInfo> nodeDisk;
    DiskManager<Data,basicInfo> dataDisk;
    // treeInfo is stored in dataDisk and updated in dataDisk
private:
    // below are private functions
public:
    // interfaces for my B+Tree
    explicit BPlusTree(string & _name):leafDisk(_name + "_leaf.dat"),nodeDisk(_name + "_node.dat"),dataDisk(_name + "_data.dat"){
        treeInfo = dataDisk.tellInfo();
    }
    ~BPlusTree(){
        dataDisk.setInfo(treeInfo);
    }
    int size() const{
        return treeInfo.size;
    }
    bool empty() const{
        return treeInfo.size == 0;
    }
    //todo
    void insert(const Key & _key,const Data & _data){

    }
    void erase(const Key & _key){

    }
    void erase(const Key & _key,const Data & _data){

    }
    void find(const Key & _key,vector<Data> & vec_ans){

    }
    void find(const Key & _key1,const Key & _key2,vector<Data> & vec_ans){

    }
};


#endif //TRAIN_TICKET_BPLUSTREE_HPP
