//
// Created by 赵先生 on 2021/4/8.
//
#ifndef TRAIN_TICKET_MYEXCEPTION_HPP
#define TRAIN_TICKET_MYEXCEPTION_HPP
#include <cstring>
class exception{
private:
    std::string content;
public:
    exception(){}
    exception(const exception & o):content(o.content){}
    exception(const char * ss):content(ss){}
    std::string what() const{
        return this->content;
    }
};
//void error(const char * ss){
//    throw exception(ss);
//}
#endif //TRAIN_TICKET_MYEXCEPTION_HPP
