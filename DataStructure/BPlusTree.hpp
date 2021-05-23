//
// Created by 赵先生 on 2021/4/8.
//

/*
 * BPlusTree.hpp
 * implement a container which transports data with disk by a prominent efficiency
 */
#ifndef TRAIN_TICKET_BPLUSTREE_HPP
#define TRAIN_TICKET_BPLUSTREE_HPP
#include "DiskManager.hpp"
#include "Myexception.hpp"
#include "functions.hpp"
#include <cstring>
#include <vector>
#include <iostream>
//#include "vector.hpp"
//#define debug // using Xiatian's debug method

using std::string;
//using sjtu::vector;
using std::vector;
using myFunctions::upper_bound;
using myFunctions::lower_bound;
using std::cout;
using std::endl;

/*
 * class Key should overload operator < and ==
 * M means the size of child in internal node
 * L represents the size of data stored in leaf node
 * C means the size of leaf node which is temporarily stored in cache
 */
template <class Key,class Data,int M = 300,int L = 130,int C = 383,int D = 14489>
class BPlusTree{
private:
    class basicInfo{
    public:
        int root = -1; //the position of rootNode
        int head = -1; //the head of the leafNode List
        int size = 0; // size of the element
    };
    class recursiveKey{
        int data[200]; // 1-base
        int ptr; // ptr points to the latest data
    public:
        recursiveKey():ptr(0){
            memset(data,0,sizeof(data));
        }
        void push_back(int _pos){
            data[++ptr] = _pos;
        }
        int pop_back(){
            return data[ptr--];
        }
        void clear(){
            ptr = 0;
        }
        int & top(){
            return data[ptr];
        }
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
            theTree->leafDisk.write(this,this->position);
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
                leafNode * tmpNode = theTree->leafDisk.read(this->rightBrother);
                tmpNode->leftBrother = newNode.position;
                theTree->leafDisk.write(tmpNode,this->rightBrother);
            }
            this->rightBrother = newNode.position;
            for(int i = 0;i < newNode.dataSize;++i){
                newNode.dataKey[i] = this->dataKey[MIN_RECORD+i];
                newNode.dataSet[i] = this->dataSet[MIN_RECORD+i];
            }
            this->dataSize = MIN_RECORD;
            theTree->leafDisk.write(newNode);
            theTree->leafDisk.write(this,this->position);
            // update fatherNode
            Node * fatherNode = (theTree->nodeDisk.read(newNode.father));
            fatherNode->addElement(newNode.dataKey[0],newNode.position,theTree);
        }
        void findElement(const Key & _key,vector<Data>& vec_ans,BPlusTree * theTree,bool left,bool right){
            int pos1 = lower_bound(this->dataKey,this->dataSize,_key);
            int pos2 = upper_bound(this->dataKey,this->dataSize,_key);
            for(int i = pos1;i <= pos2 && i < this->dataSize;++i){
                if(this->dataKey[i] == _key) vec_ans.push_back(this->dataSet[i]);
            }
            if(this->dataKey[0] == _key && this->leftBrother != -1 && left){
                leafNode * tmpNode = theTree->leafDisk.read(this->leftBrother);
                tmpNode->findElement(_key,vec_ans,theTree,true,false);
            }
            if(this->dataKey[dataSize-1] == _key && this->rightBrother != -1 && right){
                leafNode * tmpNode = theTree->leafDisk.read(this->rightBrother);
                tmpNode->findElement(_key,vec_ans,theTree,false,true);
            }
        }
        void findElement(const Key & _key,vector<Data *>& vec_ans,BPlusTree * theTree,bool left,bool right){
            int pos1 = lower_bound(this->dataKey,this->dataSize,_key);
            int pos2 = upper_bound(this->dataKey,this->dataSize,_key);
            for(int i = pos1;i <= pos2 && i < this->dataSize;++i){
                if(this->dataKey[i] == _key) vec_ans.push_back(&this->dataSet[i]);
            }
            if(this->dataKey[0] == _key && this->leftBrother != -1 && left){
                leafNode * tmpNode = theTree->leafDisk.read(this->leftBrother);
                tmpNode->findElement(_key,vec_ans,theTree,true,false);
            }
            if(this->dataKey[dataSize-1] == _key && this->rightBrother != -1 && right){
                leafNode * tmpNode = theTree->leafDisk.read(this->rightBrother);
                tmpNode->findElement(_key,vec_ans,theTree,false,true);
            }
        }

        void eraseAssistant(const Key & _key,const Data & _data,BPlusTree * theTree,bool left,bool right,int & leafPos,int & keyPos){
            int pos1 = lower_bound(this->dataKey,this->dataSize,_key);
            int pos2 = upper_bound(this->dataKey,this->dataSize,_key);
            for(int i = pos1;i <= pos2 && i < this->dataSize;++i){
                if(this->dataKey[i] == _key && this->dataSet[i] == _data){
                    leafPos = this->position;
                    keyPos = i;
                    return;
                }
            }
            if(this->dataKey[0] == _key && this->leftBrother != -1 && left && leafPos == -1){
                leafNode * tmpNode = theTree->leafDisk.read(this->leftBrother);
                tmpNode->eraseAssistant(_key,_data,theTree,true,false,leafPos,keyPos);
            }
            if(this->dataKey[dataSize-1] == _key && this->rightBrother != -1 && right && leafPos == -1){
                leafNode * tmpNode = theTree->leafDisk.read(this->rightBrother);
                tmpNode->eraseAssistant(_key,_data,theTree,false,true,leafPos,keyPos);
            }
        }

        bool borrowLeft(BPlusTree * theTree){
            if(this->leftBrother == -1) return false;
            leafNode * leftBro = theTree->leafDisk.read(this->leftBrother);
            if(leftBro->father != this->father) return false;
            if(leftBro->dataSize > MIN_RECORD){ // borrow one record from left brother
                //deal with this
                for(int i = this->dataSize;i > 0;--i){
                    this->dataKey[i] = this->dataKey[i-1];
                    this->dataSet[i] = this->dataSet[i-1];
                }
                this->dataKey[0] = leftBro->dataKey[leftBro->dataSize-1];
                this->dataSet[0] = leftBro->dataSet[leftBro->dataSize-1];
                ++this->dataSize;--leftBro->dataSize;
                //deal with father
                Node * fatherNode = theTree->nodeDisk.read(this->father);
                int pos = fatherNode->findKeyPos(leftBro->position);
                fatherNode->nodeKey[pos] = this->dataKey[0];
                //write back
                theTree->leafDisk.write(this,this->position);
                theTree->leafDisk.write(leftBro,leftBro->position);
                theTree->nodeDisk.write(fatherNode,fatherNode->position);
                return true;
            }else return false;
        }

        // using this function after borrowLeft
        bool mergeLeft(BPlusTree * theTree){
            if(this->leftBrother == -1) return false;
            leafNode * leftBro = theTree->leafDisk.read(this->leftBrother);
            if(leftBro->father != this->father) return false;
            // merge two into one
            for(int i = 0;i < this->dataSize;++i){
                leftBro->dataKey[leftBro->dataSize+i] = this->dataKey[i];
                leftBro->dataSet[leftBro->dataSize+i] = this->dataSet[i];
            }
            leftBro->dataSize += this->dataSize;
            // write back && modify the linkList
            leftBro->rightBrother = this->rightBrother;
            if(~this->rightBrother){
                leafNode * tmprr = theTree->leafDisk.read(this->rightBrother);
                tmprr->leftBrother = leftBro->position;
                theTree->leafDisk.write(tmprr,tmprr->position);
            }
            theTree->leafDisk.write(leftBro,leftBro->position);
            // deleteElement int fatherNode including writing back into document
            Node * fatherNode = theTree->nodeDisk.read(this->father);
            theTree->leafDisk.erase(this->position);
            int pos = fatherNode->findKeyPos(leftBro->position);
            fatherNode->deleteElement(pos,theTree);
            return true;
        }

        bool borrowRight(BPlusTree * theTree){
            if(this->rightBrother == -1) return false;
            leafNode * rightBro = theTree->leafDisk.read(this->rightBrother);
            if(rightBro->father != this->father) return false;
            if(rightBro->dataSize > MIN_RECORD){ // borrow one record from right brother
                //deal with this
                this->dataKey[this->dataSize] = rightBro->dataKey[0];
                this->dataSet[this->dataSize] = rightBro->dataSet[0];
                for(int i = 0;i < rightBro->dataSize - 1;++i){
                    rightBro->dataKey[i] = rightBro->dataKey[i+1];
                    rightBro->dataSet[i] = rightBro->dataSet[i+1];
                }
                ++this->dataSize;--rightBro->dataSize;
                //deal with father
                Node * fatherNode = theTree->nodeDisk.read(this->father);
                int pos = fatherNode->findKeyPos(this->position);
                fatherNode->nodeKey[pos] = rightBro->dataKey[0];
                //write back
                theTree->leafDisk.write(this,this->position);
                theTree->leafDisk.write(rightBro,rightBro->position);
                theTree->nodeDisk.write(fatherNode,fatherNode->position);
                return true;
            }else return false;
        }
        //using this function after borrowRight
        bool mergeRight(BPlusTree * theTree){
            if(this->rightBrother == -1) return false;
            leafNode * rightBro = theTree->leafDisk.read(this->rightBrother);
            if(rightBro->father != this->father) return false;
            // merge two into one
            for(int i = 0;i < rightBro->dataSize;++i){
                this->dataKey[this->dataSize+i] = rightBro->dataKey[i];
                this->dataSet[this->dataSize+i] = rightBro->dataSet[i];
            }
            this->dataSize += rightBro->dataSize;
            // write back && modify the linkList
            this->rightBrother = rightBro->rightBrother;
            if(~rightBro->rightBrother){
                leafNode * tmprr = theTree->leafDisk.read(rightBro->rightBrother);
                tmprr->leftBrother = this->position;
                theTree->leafDisk.write(tmprr,tmprr->position);
            }
            theTree->leafDisk.write(this,this->position);
            theTree->leafDisk.erase(rightBro->position);
            // deleteElement int fatherNode including writing back into document
            Node * fatherNode = theTree->nodeDisk.read(this->father);
            int pos = fatherNode->findKeyPos(this->position);
            fatherNode->deleteElement(pos,theTree);
            return true;
        }
        void deleteElement(int keyPos,BPlusTree * theTree){
            for(int i = keyPos;i < dataSize - 1;++i){
                dataKey[i] = dataKey[i+1];
                dataSet[i] = dataSet[i+1];
            }
            --dataSize;
            if(keyPos == 0 && this->position != theTree->treeInfo.head){
                Node * tmpFather = theTree->nodeDisk.read(this->father);
                int tmp = tmpFather->findKeyPos(this->position);
                while(tmp == 0){
                    int _pos = tmpFather->position;
                    tmpFather = theTree->nodeDisk.read(tmpFather->father);
                    tmp = tmpFather->findKeyPos(_pos);
                }
                tmpFather->nodeKey[tmp-1] = this->dataKey[0];
                theTree->nodeDisk.write(tmpFather,tmpFather->position);
            }
            if(dataSize > MIN_RECORD-1){
                theTree->leafDisk.write(this,this->position);
                return;
            }
            if(this->borrowLeft(theTree) || this->borrowRight(theTree)) return;
            if(this->mergeLeft(theTree) || this->mergeRight(theTree)) return;
            theTree->leafDisk.write(this,this->position);
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
            int index = theTree->rekeyPos.pop_back();
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
            theTree->nodeDisk.write(this,this->position);
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
                Node * tmpNode1 = (theTree->nodeDisk.read(this->rightBrother));
                tmpNode1->leftBrother = tmpNode.position;
                theTree->nodeDisk.write(tmpNode1,tmpNode1->position);
            }
            this->rightBrother = tmpNode.position;
            for(int i = 0;i < tmpNode.childSize;++i){
                tmpNode.childPosition[i] = this->childPosition[MIN_CHILD+i];
                // modify children's father position
                if(tmpNode.childIsLeaf){
                    leafNode * modifyFather = theTree->leafDisk.read(tmpNode.childPosition[i]);
                    modifyFather->father = tmpNode.position;
                    theTree->leafDisk.write(modifyFather,modifyFather->position);
                }else{
                    Node * modifyFather = theTree->nodeDisk.read(tmpNode.childPosition[i]);
                    modifyFather->father = tmpNode.position;
                    theTree->nodeDisk.write(modifyFather,modifyFather->position);
                }
            }
            for(int i = 0; i < tmpNode.childSize-1;++i){
                tmpNode.nodeKey[i] = this->nodeKey[MIN_CHILD+i];
            }
            // 0....MIN_CHILD-2 | MIN_CHILD-1 | MIN_CHILD.....MAX_CHILD-2
            this->childSize = MIN_CHILD; // keysize == MIN_CHILD - 1;
            theTree->nodeDisk.write(tmpNode);
            theTree->nodeDisk.write(this,this->position);
            Node * fatherNode = (theTree->nodeDisk.read(tmpNode.father));
            fatherNode->addElement(this->nodeKey[MIN_CHILD-1],tmpNode.position,theTree);
        }

        bool borrowLeft(BPlusTree * theTree){
            if(this->leftBrother == -1) return false;
            Node * leftBro = theTree->nodeDisk.read(this->leftBrother);
            if(leftBro->father != this->father) return false;
            if(leftBro->childSize > MIN_CHILD){ // borrow one node from left brother,that means child's key move upward and father key move down
                // update father node
                Node * fatherNode = theTree->nodeDisk.read(this->father);
                int keyPos = fatherNode->findKeyPos(leftBro->position);
                Key targetKey = fatherNode->nodeKey[keyPos];
                fatherNode->nodeKey[keyPos] = leftBro->nodeKey[leftBro->childSize-2];
                theTree->nodeDisk.write(fatherNode,fatherNode->position);
                // update *this
                for(int i = this->childSize-1;i > 0;--i){
                    this->nodeKey[i] = this->nodeKey[i-1];
                }
                for(int i = this->childSize;i > 0;--i){
                    this->childPosition[i] = this->childPosition[i-1];
                }
                this->nodeKey[0] = targetKey;
                this->childPosition[0] = leftBro->childPosition[leftBro->childSize-1];
                ++this->childSize;
                theTree->nodeDisk.write(this,this->position);
                // update left brother
                --leftBro->childSize;
                theTree->nodeDisk.write(leftBro,leftBro->position);
                // update child's father
                if(this->childIsLeaf){
                    leafNode * childNode = theTree->leafDisk.read(this->childPosition[0]);
                    childNode->father = this->position;
                    theTree->leafDisk.write(childNode,childNode->position);
                }else{
                    Node * childNode = theTree->nodeDisk.read(this->childPosition[0]);
                    childNode->father = this->position;
                    theTree->nodeDisk.write(childNode,childNode->position);
                }
                return true;
            }else return false;
        }
        //using this function after using borrowLeft
        bool mergeLeft(BPlusTree * theTree){
            if(this->leftBrother == -1) return false;
            Node * leftBro = theTree->nodeDisk.read(this->leftBrother);
            if(leftBro->father != this->father) return false;
            // update father node in the end
            Node * fatherNode = theTree->nodeDisk.read(this->father);
            int keyPos = fatherNode->findKeyPos(leftBro->position);
            Key downKey = fatherNode->nodeKey[keyPos];
            // merge two into one
            leftBro->nodeKey[leftBro->childSize-1] = downKey;
            for(int i = 0;i < this->childSize-1;++i){
                leftBro->nodeKey[i+leftBro->childSize] = this->nodeKey[i];
            }
            for(int i = 0;i < this->childSize;++i){
                leftBro->childPosition[i+leftBro->childSize] = this->childPosition[i];
                if(this->childIsLeaf){
                    leafNode * tmp = theTree->leafDisk.read(this->childPosition[i]);
                    tmp->father = leftBro->position;
                    theTree->leafDisk.write(tmp,tmp->position);
                }else{
                    Node * tmp = theTree->nodeDisk.read(this->childPosition[i]);
                    tmp->father = leftBro->position;
                    theTree->nodeDisk.write(tmp,tmp->position);
                }
            }
            leftBro->childSize += this->childSize;
            // write back && modify the linkList
            leftBro->rightBrother = this->rightBrother;
            if(~this->rightBrother){
                Node * tmprr = theTree->nodeDisk.read(this->rightBrother);
                tmprr->leftBrother = leftBro->position;
                theTree->nodeDisk.write(tmprr,tmprr->position);
            }
            theTree->nodeDisk.write(leftBro,leftBro->position);
            theTree->nodeDisk.erase(this->position);
            // delete the element in father node
            fatherNode->deleteElement(keyPos,theTree);
            return true;
        }

        bool borrowRight(BPlusTree * theTree){
            if(this->rightBrother == -1) return false;
            Node * rightBro = theTree->nodeDisk.read(this->rightBrother);
            if(rightBro->father != this->father) return false;
            if(rightBro->childSize > MIN_CHILD){ // borrow one node from left brother,that means child's key move upward and father key move down
                // update father node
                Node * fatherNode = theTree->nodeDisk.read(this->father);
                int keyPos = fatherNode->findKeyPos(this->position);
                // there was a bug ~~~~2333
                this->nodeKey[this->childSize-1] = fatherNode->nodeKey[keyPos];
                fatherNode->nodeKey[keyPos] = rightBro->nodeKey[0];
                theTree->nodeDisk.write(fatherNode,fatherNode->position);
                // update *this
                this->childPosition[this->childSize] = rightBro->childPosition[0];
                ++this->childSize;
                theTree->nodeDisk.write(this,this->position);
                // update left brother
                for(int i = 0;i < rightBro->childSize - 2;++i){
                    rightBro->nodeKey[i] = rightBro->nodeKey[i+1];
                }
                for(int i = 0;i < rightBro->childSize - 1;++i){
                    rightBro->childPosition[i] = rightBro->childPosition[i+1];
                }
                --rightBro->childSize;
                theTree->nodeDisk.write(rightBro,rightBro->position);
                // update child's father
                if(this->childIsLeaf){
                    leafNode * childNode = theTree->leafDisk.read(this->childPosition[this->childSize-1]);
                    childNode->father = this->position;
                    theTree->leafDisk.write(childNode,childNode->position);
                }else{
                    Node * childNode = theTree->nodeDisk.read(this->childPosition[this->childSize-1]);
                    childNode->father = this->position;
                    theTree->nodeDisk.write(childNode,childNode->position);
                }
                return true;
            }else return false;
        }
        //using this function after using borrowRight
        bool mergeRight(BPlusTree * theTree){
            if(this->rightBrother == -1) return false;
            Node * rightBro = theTree->nodeDisk.read(this->rightBrother);
            if(rightBro->father != this->father) return false;
            // update father node in the end
            Node * fatherNode = theTree->nodeDisk.read(this->father);
            int keyPos = fatherNode->findKeyPos(this->position);
            Key downKey = fatherNode->nodeKey[keyPos];
            // merge two into one
            this->nodeKey[this->childSize-1] = downKey;
            for(int i = 0;i < rightBro->childSize-1;++i){
                this->nodeKey[i+this->childSize] = rightBro->nodeKey[i];
            }
            for(int i = 0;i < rightBro->childSize;++i){
                this->childPosition[i+this->childSize] = rightBro->childPosition[i];
                if(this->childIsLeaf){
                    leafNode * tmp = theTree->leafDisk.read(rightBro->childPosition[i]);
                    tmp->father = this->position;
                    theTree->leafDisk.write(tmp,tmp->position);
                }else{
                    Node * tmp = theTree->nodeDisk.read(rightBro->childPosition[i]);
                    tmp->father = this->position;
                    theTree->nodeDisk.write(tmp,tmp->position);
                }
            }
            this->childSize += rightBro->childSize;
            // write back && modify the linkList
            this->rightBrother = rightBro->rightBrother;
            if(~rightBro->rightBrother){
                Node * tmprr = theTree->nodeDisk.read(rightBro->rightBrother);
                tmprr->leftBrother = this->position;
                theTree->nodeDisk.write(tmprr,tmprr->position);
            }
            theTree->nodeDisk.write(this,this->position);
            theTree->nodeDisk.erase(rightBro->position);
            // delete the element in father node
            fatherNode->deleteElement(keyPos,theTree);
            return true;
        }
        void deleteElement(int keyPos,BPlusTree * theTree){
            for(int i = keyPos;i < childSize - 2;++i){
                this->nodeKey[i] = this->nodeKey[i+1];
            }
            for(int i = keyPos + 1;i < childSize - 1;++i){
                this->childPosition[i] = this->childPosition[i+1];
            }
            --childSize;
            if(this->position == theTree->treeInfo.root){
                if(this->childSize == 1 && !this->childIsLeaf){ // relocate the root
                    theTree->treeInfo.root = this->childPosition[0];
                    Node * newRoot = theTree->nodeDisk.read(this->childPosition[0]);
                    newRoot->father = -1;
                    theTree->nodeDisk.write(newRoot,newRoot->position);
                    return;
                }else{
                    theTree->nodeDisk.write(this,this->position);
                    return;
                }
            }
            if(childSize > MIN_CHILD-1){
                theTree->nodeDisk.write(this,this->position);
                return;
            }
            if(this->borrowLeft(theTree) || this->borrowRight(theTree)) return;
            if(this->mergeLeft(theTree) || this->mergeRight(theTree)) return;
            theTree->nodeDisk.write(this,this->position);
        }

        int findKeyPos(int sonPos){
            for(int i = 0; i < this->childSize;++i){
                if(this->childPosition[i] == sonPos) return i;
            }
            std::cerr << "can not find the position";
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
    recursiveKey rekeyPos;
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
        rekeyPos.clear();
        Node * tmpNode = nodeDisk.read(this->treeInfo.root);
        while(!tmpNode->childIsLeaf){
            int index = upper_bound(tmpNode->nodeKey,tmpNode->childSize-1,_key);
            rekeyPos.push_back(index);
            tmpNode = nodeDisk.read(tmpNode->childPosition[index]);
        }
        int index = upper_bound(tmpNode->nodeKey,tmpNode->childSize-1,_key);
        rekeyPos.push_back(index);
        return tmpNode->childPosition[index];
    }
public:
    // interfaces for my B+Tree
    BPlusTree() = delete;
    explicit BPlusTree(const string & _name):leafDisk(_name + "_leaf.dat",C),nodeDisk(_name + "_node.dat",D){
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
    //parameter: the key and  the data object itself
    void insert(const Key & _key,const Data & _data){
        treeInfo.size++;
        if(treeInfo.root == -1) createRoot(_key,_data);
        else{
            int leafPos = findLeaf(_key);
            leafNode * tmpLeaf = leafDisk.read(leafPos);
            tmpLeaf->addElement(_key,_data,this);
        }
    }
    // delete the specific data with the key
    bool erase(const Key & _key,const Data & _data){
        if(treeInfo.root == -1) return false;
        int leafPos = -1,keyPos = -1;
        int leafPosition = findLeaf(_key);
        leafNode * tmpLeafNode = leafDisk.read(leafPosition);
        tmpLeafNode->eraseAssistant(_key,_data,this,true,true,leafPos,keyPos);
        if(leafPos == -1) return false;
        if(treeInfo.size == 1){
            this->clear();
            return true;
        }
        leafNode * targetLeafNode = leafDisk.read(leafPos);
        targetLeafNode->deleteElement(keyPos,this);
        --treeInfo.size;
        return true;
    }
    // find all data associated with the key
    void find(const Key & _key,vector<Data> & vec_ans){
        if(treeInfo.root == -1) return;
        int leafPosition = findLeaf(_key);
        leafNode * tmpLeafNode = leafDisk.read(leafPosition);
        tmpLeafNode->findElement(_key,vec_ans,this,true,true);
    }
    // delete all data and reset the document
    void clear(){
        leafDisk.clear();
        nodeDisk.clear();
        treeInfo.root = -1;
        treeInfo.head = -1;
        treeInfo.size = 0;
    }
    void findAll(vector<Data> & vec_ans){
        if(treeInfo.root == -1 || treeInfo.head == -1) return;
        int ptr = treeInfo.head;
        while(true){
            leafNode * tmpNode = this->leafDisk.read(ptr);
            for(int i = 0;i < tmpNode->dataSize;++i){
                vec_ans.push_back(tmpNode->dataSet[i]);
            }
            if(tmpNode->rightBrother == -1) break;
            ptr = tmpNode->rightBrother;
        }
    }

    // this function provides a way to modify the data without insert and erase.
    // Attention : 1. make sure that _data exists 2. modify it at once
    Data & update(const Key & _key,const Data & _data){
        if(treeInfo.root == -1 || treeInfo.head == -1) throw "update debug #1";
        int leafPos = -1,keyPos = -1;
        int leafPosition = findLeaf(_key);
        leafNode * tmpLeafNode = leafDisk.read(leafPosition);
        tmpLeafNode->eraseAssistant(_key,_data,this,true,true,leafPos,keyPos);
        if(leafPos == -1) throw "update debug #2";
        leafNode * targetLeafNode = leafDisk.read(leafPos,true);
        return targetLeafNode->dataSet[keyPos];
    }
#ifdef debug
private:
    void show(int offset, bool isLeaf)  {
        cout << "[pos] " << offset << endl;
        if (isLeaf) {
            leafNode * tempNode = leafDisk.read(offset);
            tempNode->show();
        }
        else {
            Node * tempNode = nodeDisk.read(offset);
            tempNode->show();
            cout << endl;
            for (int i = 0; i < tempNode->childSize; i++) {
                if (tempNode->childIsLeaf) show(tempNode->childPosition[i], true);
                else show(tempNode->childPosition[i], false);
            }
        }
    };

public:
    void show() {
        if(treeInfo.root == -1){
            cout << "[show] tree empty" << endl;
            return;
        }
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
