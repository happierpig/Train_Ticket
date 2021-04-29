//
// Created by 赵先生 on 2021/4/8.
//

#ifndef TRAIN_TICKET_BPLUSTREE_HPP
#define TRAIN_TICKET_BPLUSTREE_HPP
#include "DiskManager.hpp"
#include "Myexception.hpp"
#include "functions.hpp"
#include <cstring>
#include <vector>

#define debug // using Xiatian's debug method

using std::string;
using std::vector;
using myFunctions::upper_bound;
using myFunctions::lower_bound;
using std::cout;
using std::endl;

template <class Key,class Data,int M = 200,int L = 200>
class BPlusTree{
private:
    class basicInfo{
    public:
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
        Data dataSet[MAX_RECORD]; // data is directly stored in leaf node which accelerate the speed of reading disk
        void addElement(const Key & _key,const Data & _data,BPlusTree * theTree){
            int index = upper_bound(dataKey,dataSize,_key);
            for(int i = dataSize;i > index;--i){
                dataKey[i] = dataKey[i-1];
                dataSet[i] = dataSet[i-1];
            }
            dataKey[index] = _key;
            dataSet[index] = _data;
            dataSize++;
            if(dataSize == MAX_RECORD){
                this->splitNode(theTree);
                return;
            }
            theTree->leafDisk.write(*this,this->position);
        }
        void splitNode(BPlusTree * theTree){
            //initialize the newLeafNode
            leafNode newNode;
            newNode.dataSize = MAX_RECORD - MIN_RECORD;
            newNode.position = theTree->leafDisk.tellp();
            newNode.father = this->father;
            newNode.leftBrother = this->position;
            newNode.rightBrother = this->rightBrother;
            if(this->rightBrother >= 0){
                leafNode tmpNode = theTree->leafDisk.read(this->rightBrother);
                tmpNode.leftBrother = newNode.position;
                theTree->leafDisk.write(tmpNode,this->rightBrother);
            }
            this->rightBrother = newNode.position;
            for(int i = 0;i < newNode.dataSize;++i){
                newNode.dataKey[i] = this->dataKey[MIN_RECORD+i];
                newNode.dataSet[i] = this->dataSet[MIN_RECORD+i];
            }
            this->dataSize = MIN_RECORD;
            theTree->leafDisk.write(newNode);
            theTree->leafDisk.write(*this,this->position);
            // update fatherNode
            Node fatherNode = (theTree->nodeDisk.read(newNode.father));
            fatherNode.addElement(newNode.dataKey[0],newNode.position,theTree);
        }
        void findElement(const Key & _key,vector<Data>& vec_ans,BPlusTree * theTree,bool left,bool right){
            int pos1 = lower_bound(this->dataKey,this->dataSize,_key);
            int pos2 = upper_bound(this->dataKey,this->dataSize,_key);
            for(int i = pos1;i <= pos2 && i < this->dataSize;++i){
                if(this->dataKey[i] == _key) vec_ans.push_back(this->dataSet[i]);
            }
            if(this->dataKey[0] == _key && this->leftBrother != -1 && left){
                leafNode tmpNode = theTree->leafDisk.read(this->leftBrother);
                tmpNode.findElement(_key,vec_ans,theTree,true,false);
            }
            if(this->dataKey[dataSize-1] == _key && this->rightBrother != -1 && right){
                leafNode tmpNode = theTree->leafDisk.read(this->rightBrother);
                tmpNode.findElement(_key,vec_ans,theTree,false,true);
            }
        }

#ifdef debug

        void show() const {
                cout << "[leafNode]" << endl;
                cout << "position: " << position << endl;
                cout << "father: " << father << endl;
                cout << "leftBrother: " << leftBrother << endl;
                cout << "rightBrother: " << rightBrother << endl;
                cout << "dataSize: " << dataSize << endl;
                cout << "leafKey & leafData:" << endl;
                for (int i = 0; i < dataSize; i++) {
                    cout << "leafKey: " << dataKey[i] << "\t\t\t\t\t\t\t\t\t\t\t" << "leafData: " << dataSet[i] << endl;
                }
                cout << endl;
        }
#endif

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
        void addElement(const Key & _key,int _position,BPlusTree * theTree){
            int index = upper_bound(this->nodeKey,childSize-1,_key);
            for(int i = childSize-1;i > index;--i){
                nodeKey[i] = nodeKey[i-1];
            }
            nodeKey[index] = _key;
            for(int i = childSize;i > index+1;--i){
                childPosition[i] = childPosition[i-1];
            }
            childPosition[index+1] = _position;
            ++childSize;
            if(childSize == MAX_CHILD){
                this->splitNode(theTree);
                return;
            }
            theTree->nodeDisk.write(*this,this->position);
        }
        void splitNode(BPlusTree * theTree){
            //relocation the root information
            if(this->position == theTree->treeInfo.root){
                Node newRoot;
                newRoot.position = theTree->nodeDisk.tellp();
                newRoot.childSize = 1;
                newRoot.childPosition[0] = this->position;
                theTree->nodeDisk.write(newRoot);
                theTree->treeInfo.root = newRoot.position;
                this->father = theTree->treeInfo.root;
            }
            Node tmpNode;
            tmpNode.father = this->father;
            tmpNode.position = theTree->nodeDisk.tellp();
            tmpNode.leftBrother = this->position;tmpNode.rightBrother = this->rightBrother;
            tmpNode.childSize = MAX_CHILD - MIN_CHILD; // keysize = MAX_CHILD - MIN_CHILD - 1
            tmpNode.childIsLeaf = this->childIsLeaf;
            if(this->rightBrother >= 0){
                Node tmpNode1 = (theTree->nodeDisk.read(this->rightBrother));
                tmpNode1.leftBrother = tmpNode.position;
                theTree->nodeDisk.write(tmpNode1,tmpNode1.position);
            }
            this->rightBrother = tmpNode.position;
            for(int i = 0;i < tmpNode.childSize;++i){
                tmpNode.childPosition[i] = this->childPosition[MIN_CHILD+i];
                // modify children's father position
                if(tmpNode.childIsLeaf){
                    leafNode modifyFather = theTree->leafDisk.read(tmpNode.childPosition[i]);
                    modifyFather.father = tmpNode.position;
                    theTree->leafDisk.write(modifyFather,modifyFather.position);
                }else{
                    Node modifyFather = theTree->nodeDisk.read(tmpNode.childPosition[i]);
                    modifyFather.father = tmpNode.position;
                    theTree->nodeDisk.write(modifyFather,modifyFather.position);
                }
            }
            for(int i = 0; i < tmpNode.childSize-1;++i){
                tmpNode.nodeKey[i] = this->nodeKey[MIN_CHILD+i];
            }
            // 0....MIN_CHILD-2 | MIN_CHILD-1 | MIN_CHILD.....MAX_CHILD-2
            this->childSize = MIN_CHILD; // keysize == MIN_CHILD - 1;
            theTree->nodeDisk.write(tmpNode);
            theTree->nodeDisk.write(*this,this->position);
            Node fatherNode = (theTree->nodeDisk.read(tmpNode.father));
            fatherNode.addElement(this->nodeKey[MIN_CHILD-1],tmpNode.position,theTree);
        }

#ifdef debug

        void show() const {
            cout << "[internalNode]" << endl;
            cout << "position: " << position << endl;
            cout << "father: " << father << endl;
            cout << "leftBrother: " << leftBrother << endl;
            cout << "rightBrother: " << rightBrother << endl;
            cout << (childIsLeaf ? "child node is leaf" : "child node is internal") << endl;
            cout << "childSize: " << childSize << endl;
            cout << "nodeKey & childPosition:" << endl;
            for (int i = 0; i < childSize - 1; i++) {
                cout <<  "childNode: " << childPosition[i] << endl;
                cout << "nodeKey: " << nodeKey[i] << endl;
            }
            cout  << "childNode: " << childPosition[childSize-1] << endl;
            cout << endl;

        }

#endif
    };
private:
    //below are private members of the tree
    basicInfo treeInfo;
    DiskManager<leafNode,basicInfo> leafDisk;
    DiskManager<Node,basicInfo> nodeDisk;
    // treeInfo is stored in nodeDisk and updated in nodeDisk
private:
    // below are private functions
    void createRoot(const Key & _key,const Data & _data){
        //initialize the newRootNode
        Node newRoot;
        newRoot.position = nodeDisk.tellp();
        ++newRoot.childSize;
        newRoot.childIsLeaf = true;
        newRoot.childPosition[0] = leafDisk.tellp();
        //initialize the leafNode
        leafNode newLeafNode;
        newLeafNode.position = newRoot.childPosition[0];
        newLeafNode.father = newRoot.position;
        ++newLeafNode.dataSize;
        newLeafNode.dataKey[0] = _key;
        newLeafNode.dataSet[0] = _data;
        treeInfo.root = newRoot.position;
        treeInfo.head = newLeafNode.position;
        treeInfo.size = 1;
        //write in diskManager
        nodeDisk.write(newRoot);
        leafDisk.write(newLeafNode);
    }
    int findLeaf(const Key & _key){
        if(treeInfo.root == -1) error("树为空");
        Node tmpNode = nodeDisk.read(this->treeInfo.root);
        while(!tmpNode.childIsLeaf){
            int index = upper_bound(tmpNode.nodeKey,tmpNode.childSize-1,_key);
            tmpNode = nodeDisk.read(tmpNode.childPosition[index]);
        }
        int index = upper_bound(tmpNode.nodeKey,tmpNode.childSize-1,_key);
        return tmpNode.childPosition[index];
    }
public:
    // interfaces for my B+Tree
    BPlusTree() = delete;
    explicit BPlusTree(const string & _name):leafDisk(_name + "_leaf.dat"),nodeDisk(_name + "_node.dat"){
        treeInfo = nodeDisk.tellInfo();
    }
    ~BPlusTree(){
        nodeDisk.setInfo(treeInfo);
    }
    int size() const{
        return treeInfo.size;
    }
    bool empty() const{
        return treeInfo.size == 0;
    }
    //parameter: the key and the data object itself
    void insert(const Key & _key,const Data & _data){
        treeInfo.size++;
        if(treeInfo.root == -1) createRoot(_key,_data);
        else{
            int leafPos = findLeaf(_key);
            leafNode tmpLeaf = leafDisk.read(leafPos);
            tmpLeaf.addElement(_key,_data,this);
        }
    }
    // delete the specific data with the key
    bool erase(const Key & _key,const Data & _data){

    }
    // find all data associated with the key
    void find(const Key & _key,vector<Data> & vec_ans){
        int leafPosition = findLeaf(_key);
        leafNode tmpLeafNode = leafDisk.read(leafPosition);
        tmpLeafNode.findElement(_key,vec_ans,this,true,true);
    }
    // find all data which owns key ranged from key1 to key2
    void find(const Key & _key1,const Key & _key2,vector<Data> & vec_ans){

    }



#ifdef debug
private:
    void show(int offset, bool isLeaf)  {
        cout << "[pos] " << offset << endl;
        if (isLeaf) {
            leafNode tempNode = leafDisk.read(offset);
            tempNode.show();
        }
        else {
            Node tempNode = nodeDisk.read(offset);
            tempNode.show();
            cout << endl;
            for (int i = 0; i < tempNode.childSize; i++) {
                if (tempNode.childIsLeaf) show(tempNode.childPosition[i], true);
                else show(tempNode.childPosition[i], false);
            }
        }
    };

public:
    void show() {
        cout << "[show]--------------------------------------------------------------------------------" << endl;
        show(treeInfo.root, false);
        cout << "[show]--------------------------------------------------------------------------------" << endl;
    }

    void showLeaves() const {
        int cur = treeInfo.head;
        while (cur >= 0) {
            leafNode nowNode = leafDisk.read(cur);
            nowNode.show();
            cur = nowNode.rightBrother;
        }
    }

#endif
};


#endif //TRAIN_TICKET_BPLUSTREE_HPP
