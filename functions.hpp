//
// Created by 赵先生 on 2021/4/26.
//

#ifndef TRAIN_TICKET_FUNCTIONS_HPP
#define TRAIN_TICKET_FUNCTIONS_HPP
namespace myFunctions{

    // find the minimum number which is larger than _target
    template<class T>
    int upper_bound(const T * const head,int size,const T & _target){
        if(!size) return 0;
        if(_target < head[0]) return 0;
        if(_target >= head[size-1]) return size;
        int l = 0,r = size - 1;
        while(l < r){
            int mid = (l+r) >> 1;
            if(_target < head[mid]) r = mid;
            else l = mid + 1;
        }
        return l;
    }

    template<class T>
    int lower_bound(const T * const head,int size,const T & _target){
        if(!size) return 0;
        if(_target <= head[0]) return 0;
        if(_target > head[size-1]) return size-1;
        int l = 0,r = size - 1;
        while(l < r){
            int mid = (l+r+1) >> 1;
            if(_target > head[mid]) l = mid;
            else r = mid - 1;
        }
        return l;
    }

}
#endif //TRAIN_TICKET_FUNCTIONS_HPP
