# 数据结构使用指南

> DataStructure 部分由happypig完成

## vector

为本学期第一次大作业中的sjtu::vector**修改**成型

不提供迭代器以及相关操作

### 接口：

- 默认构造函数

- []重载
- bool empty()
- size_t size()
- void push_back(const Data &)
- clear()

## HashMap

是一个采用拉链法(链表)实现的哈希表

同时可以选择使用*STL中的hash函数*或者使用者自己传入一个hash仿函数

> include <functional>
>
> std::hash<key>

### 类设计：

- class LinkList

  最普通的单向链表 用来保存发生hash碰撞相同key值的元素

- LinkList * dataSet

  是一个链表数组 有capacity个链表来保存对应key值的元素

### 原理：

当发生哈希碰撞时将该数据加入到当前hash(key)对应链表的末尾。

### 模版：

```c++
template<class Key,class Data,class Hash = std::hash<Key>>
```

使用时需要**显式**指明哈希表的key类型和data类型

可以选择传入key值的hash仿函数或者使用默认的std中的hash函数

### 接口：

- 默认构造函数= delete

- 带一个int参数的构造函数：需要使用者指明链表的个数

  最好是质数

- bool exist(const Key &)

- void insert(const Key & ,const Data &)

- void erase(const Key &)

- Data & find(const Key &)

- 重载[]；参数为const Key &、返回值为 Data &

- clear() 清空哈希表

- int count(const Key &)

  Exist->return 1;else->return 0;

## functions

### 接口：

- ```c++
  int upper_bound(const T * const head,int size,const T & _target)
  ```

  给出数据组的头指针、数据的个数、以及目标的值

  返回第一个比该目标大的数据的下标

- ```c++
      int lower_bound(const T * const head,int size,const T & _target)
  ```

  给出数据组的头指针、数据的个数、以及目标的值

  返回第一个比该目标小的数据的下标

  > ps：T类型需要重载 < 和 == 符号

- ```c++
  void sort(vector<T> & vec,int l,int r,bool (*cmp)(T,T))
  ```

  为vector设计的归并排序

## DiskManager

该类封装了b+树与文件交互的部分。其中封装了文件回收和cache(LRU)

b+树中通过调用*DiskManager*的read()和write()函数进行获取数据

### 类设计：

- class List{}

  是一个双向链表 用来维护缓存中存储的数据元素

- HashMap：用来实现高效的判断存在和调取元素
- int nowPtr,nextPtr: 用来存储可用的文件位置(文件空间回收)

### 原理：

- 文件回收：

  将erase调的data的位置通过tellp()记录下来

  将上一个待用的位置写在该位置上形成一个文件链表

  每当需要一片新的空间写数据时，先看看文件链表是否为-1

- LRU cache:

  - 用一个双向链表维护数据。双向链表中的节点包括数据、位置

  - 用hash表维护<int,Node *> 从而实现通过数据的文件位置快速的调用数据而不需要在双向链表中遍历查找

  - 每次被使用后，结点要被放到开头，以保证末尾的结点是多次没有被使用过的

  - 当b+树申请使用一个结点时，通过哈希表判断该节点是否在内存中，如果在就通过哈希表快速的获取到数据，如果不在就将数据从文件中读取出来并存到双向链表首处和哈希表当中，当达到最大数量时还要将末尾的结点写回文件。

  - 一个小小的优化：

    将双向链表中的节点打上一个bool值，只有当节点中的数据被修改后，bool值设置为true，最后才用将其写入文件当中。

- 文件读写：封装函数

  ```c++
  file.read(reinterpret<char *>(&x),sizeof(x));
  file.write(reinterpret<char *>(&x),sizeof(x));
  ```

### 接口：

- ```c++
  explicit DiskManager(const string & _name,int _capacity):fileName(_name),cache(_capacity,this),assistantMap(_capacity)
  ```

  需要使用者给出文件存储的名字和缓存的大小

- ```c++
  int write(const Data &)
  ```

  写入文件一个新的数据并且返回文件位置

- ```c++
  void write(const Data & ,int position)
  ```

  在给定位置写数据

- ```c++
  T * read(int index,bool mode = false) 
  ```

  从位置为index的地方读出来一个数据

- ```c++
  void erase(int index)
  ```

  删除位置为index的地方的数据，并将位置加入文件链表当中

  > 以上函数都内嵌了cache的操作

- Clear()

  清空整个文件。恢复出厂设置

## BPlusTree

实现为可重复key。文件读写部分采用DiskManager封装起来

### 类设计：

- class basicInfo

  保存树的基本信息：根的位置 头的位置以及数据数量

  在构造和析构时对文件进行读写以维护数据的可复用(第一次打开文件特判)

- class recursiveKey

  是一个简单的栈

  目的：插入时顺着索引一路向下将路上的索引压入栈中，在插入后进行的各种操作不用再寻找节点对应的上一层key值是第几个

  删除时不能使用

- class leafNode;class Node

  叶子节点和内部节点

  内置了删除和插入时需要进行的操作：合并、分裂、借儿子

- DiskManager leafDisk,nodeDisk;

  使用DiskManager类对b+树进行的文件操作进行封装

### 接口：

- ```c++
  template <class Key,class Data,int M = 250,int L = 100,int C = 311,int D = 14939>
  ```

  M：内部节点最大节点个数；L：儿子节点最大数据个数；C：缓存中存储的叶子节点最大个数；D：缓存中存储的内部节点的最大个数

  > 参数由计算+炼丹得到

- ```c++
  void insert(const Key & _key,const Data & _data)
  ```

  插入一个数据

- ```c++
  bool erase(const Key & _key,const Data & _data)
  ```

  删除一个数据 并返回一个bool值表示是否删除成功

- ```c++
  void find(const Key & _key,vector<Data> & vec_ans)
  ```

  找到key值符合的所有数据并塞入到vec当中

  传参vector & 避免了大量赋值

- ```c++
  void clear()
  ```

  将整颗b+树清空恢复出厂设置

- ```c++
  void findAll(vector<Data> & vec_ans)
  ```

  将b+树所有的数据都塞入到vector当中

  通过最底层叶子结点的链表实现

- ```c++
  Data & update(const Key & _key,const Data & _data)
  ```

  功能：实现数据在b+树中的modify，而不需要进行删除和再插入

  原理：利用cache实现，返回cache中存储的数据引用

  ps：需要在获得引用后立刻进行修改，避免该数据被写回文件当中被delete掉

