#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include <climits>
#include <cstddef>

namespace sjtu {
    const static int initSize = 20;
template<typename T>
class vector {
private:
    int maxElem;
    int curElem;
    T ** data;

    void doubleSpace(){
        T ** temp = data;
        maxElem *= 2;
        data = new T*[maxElem];
        for(int i = 0; i < curElem; ++i) data[i] = temp[i]; // unnecessery to new
        for(int i = curElem;i < maxElem;++i) data[i] = nullptr;
        delete [] temp;
    }
    void clear_TA() {
        if(this->data != nullptr){
            for(int i = 0;i < maxElem;++i){
                if(this->data[i] != nullptr) delete data[i];
            }
            delete [] data;
        }
        this->curElem = 0;this->maxElem = 0;
        data = nullptr;
    }
public:

	vector():maxElem(initSize),curElem(0){
	    data = new T* [initSize];
	    for(int i = 0;i < maxElem;++i) data[i] = nullptr;
	}

	vector(const vector &other) {
	    this->maxElem = other.maxElem;
	    this->curElem = other.curElem;
	    this->data = new T*[this->maxElem];
	    for(int i = 0;i < this->curElem;++i) this->data[i] = new T(*(other.data[i]));
	    for(int i = curElem;i < maxElem;++i) this->data[i] = nullptr;
	}

	~vector() {
        this->clear_TA();
	}

	vector &operator= (const vector &other) {
        if(this == &other) return *this;
        this->clear_TA();
        this->maxElem = other.maxElem;
        this->curElem = other.curElem;
        this->data = new T*[this->maxElem];
        for(int i = 0;i < this->curElem;++i) this->data[i] = new T(*(other.data[i]));
        for(int i = curElem;i < maxElem;++i) this->data[i] = nullptr;
        return *this;
	}

	T & operator[](const size_t &pos) {
        if(pos < 0 || pos >= curElem) throw "[Vector Error]index_out_of_bound";
        return *(data[pos]);
	}

	const T & operator[](const size_t &pos) const {
        if(pos < 0 || pos >= curElem) throw "[Vector Error]index_out_of_bound";
        return *(data[pos]);
	}

	bool empty() const {
	    return (curElem == 0);
	}

	size_t size() const {
         return curElem;
	}

	void push_back(const T &value) {
        data[this->curElem++] = new T(value);
        if (this->curElem == this->maxElem) doubleSpace();
    }

    void clear(){
        this->clear_TA();
        this->maxElem = initSize;
        this->curElem = 0;
        this->data = new T* [initSize];
        for(int i = 0;i < maxElem;++i) data[i] = nullptr;
    }
};
}

#endif
