
#ifndef TICKETSYSTEM_2021_MAIN_ALL_HEADER_H
#define TICKETSYSTEM_2021_MAIN_ALL_HEADER_H

#include "DataStructure/BPlusTree.hpp"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <cmath>
#include <map>
#include <unordered_map>
#include <vector>
#include <algorithm>

#define MAX_STR_LENGTH 51
#define MAX_STATION_SUM 105
#define MAX_DATE 100
#define CORE_FILE "core_file.dat"
#define USER_FILE "user_file"
#define TRAIN_FILE "train_file"
#define DEAL_FILE "deal_file"
#define LOCATION_FILE "location_file"
#define WAITING_LIST_FILE "waiting_list_file"
#define DAY_TRAIN_FILE "day_train_file"
#define REAL_TRAIN_FILE "real_train_file.dat"
#define REAL_USER_FILE "real_user_file.dat"
#define REAL_DAY_TRAIN_FILE "real_day_train_file.dat"
#define MAX_MONEY_COST 2000000000

using namespace std ;

int get_strip( string &temp_str ) ; // 中间条纹数

bool isNumber( string temp_str ) ; // 判断数字

void get_split_context( string &input_str , stringstream &temp_stream ) ;

const int all_month_add_up[13] = {0,0,44640,84960,129600,172800,217440,260640,305280,349920,393120,437760,480960}; // 月份前缀和

enum ticket_status{ succeed , pending , refunded };

int str_to_int( const string &other ) ;

class train ;

class date
{
//    friend class train ;
private:
//    int month = 0 ; // todo 重构成单 int 来进行操作
//    int day = 0 ;
//    int hour = 0 ;
//    int minute = 0 ;

    int all_time = 0 ; //  01-01 00:00 为 0

public:

    date(){} ;

    date( string &date_str ) ; // 仅给出日期

    date( string &date_str , string &time_str ) ; // 给出两个部分

    void add_day() ;

    void add_hour() ;

    void add_minute() ;

    void del_day() ;

    void del_hour() ;

    void del_minute() ;

    void become_last_minute() ;

    void become_first_minute() ;

    date temp_first_minute() ;

    date temp_last_minute() ;

    int get_month() ;

    int get_day() ;

    int get_hour() ;

    int get_minute() ;

    date operator+( int interval_time ) ;

    date operator-( int interval_time ) ;

    int operator-( date other_date ) const ; // todo 调用次数不多，采用 add_day -> add_hour -> add_minute 分步得答案

    bool operator<( const date &other ) const ;

    bool operator>( const date &other ) const ;

    bool operator==( const date &other ) const ;

    int get_date_index() ; // 1-base

    bool isSameDay( date other_date ) ;

    void get_other_day( date other_date ) ;

    void get_other_time( date other_date ) ;

    void print_date() ; // 4 参数为 0 则 xx-xx xx:xx

    friend ostream &operator<<( ostream &os , const date &temp_date ) ;

};

class my_system ;
class IndexKey ;

class para
{
public:
    string c , u , p , n , m , g , i , s , x , t , o , d , y , q , f ;

public:

    para(){} ;

    para( stringstream &input_stream ) ;

};

// todo 修改所有的赋值

class user
{
    friend class my_system ;
    friend class IndexKey ;
    friend class ticket_deal ;
private:
    char user_name[21] = {0} ;
    char password[31] = {0} ;
    char chinese_name[22] = {0} ;
    char mailAddr[MAX_STR_LENGTH] = {0};
    unsigned short privilege = 0 ;
//    int deal_sum = 0 ;

public:

    user(){} ;

    user( para &input_para ) ;

    bool operator<( const user &other_user ) const ;

    bool operator==( const user &other_user ) const ;

    bool operator>( const user &other_user ) const ;

    user& operator=( const user &other_user ) ;

    bool right_password( string &input_password ) ;

    void print_user() ;

//    void change_deal() ;

    friend ostream &operator<<( ostream &os , const user &temp_user ) ;

};

struct day_train{
    int seat_num[MAX_STATION_SUM] = {0} ;
    int get_max_available_ticket( int location_1 , int location_2 ) ;
    void modify_seat( int location_1 , int location_2 , int ticket_num ) ;
    day_train &operator=( const day_train &other ) ;
    bool operator<( const day_train &other ) const ;
    bool operator==( const day_train &other ) const ;
    bool operator>( const day_train &other ) const ;
};

class train
{
    friend class my_system ;
    friend class IndexKey ;
    friend class ride ;
    friend class ticket_deal ;
private:
    bool isReleased = false ;
    char trainID[21] = {0} ;
    char train_type = 0 ;
    char all_station[MAX_STATION_SUM][42] = {0} ; // station_sum 1-base
    int seat_num = 0 ;
    int station_num = 0 ;
    int all_price[MAX_STATION_SUM] = {0}; // 到第 i 站所要的钱 前缀和
//    int all_departure[MAX_STATION_SUM] = {0};
//    int all_arrival[MAX_STATION_SUM] = {0}; // todo 删掉 all_departure all_arrival  all_seat
//    int all_seat[MAX_DATE][MAX_STATION_SUM] = {0}; // 从哪里出发扣哪里车票
//    int waiting_length = 0 ;
    date all_set_off[MAX_STATION_SUM] , all_arrive_in[MAX_STATION_SUM] ;
    date sale_begin , sale_end ;

public:

    train(){} ;

    train( para &input_para ) ; // todo 需要拷贝构造？

    train &operator=( const train &other ) ; // todo 避免调用

    bool operator<( const train &other_train ) const ;

    bool operator==( const train &other_train ) const ;

    bool operator>( const train &other_train ) const ;

    void release_train() ;

//    void change_waiting_length( int i ) ; // +1

    int get_location( string &input_location ) ; // 未找到返回 -1

    bool is_released() ;

    bool in_sale( date purchase_day , int location ) ;

    bool can_take_in_time( date &arrive_in_date , int location ) ;

//    void ticket_decrease( date purchase_day , int location_1 , int location_2 , int purchase_ticket ) ; // 消耗 purchase_day 从 location_1 到 location_2 的车票

//    void ticket_increase( date purchase_day , int location_1 , int location_2 , int purchase_ticket ) ;

//    bool ticket_is_enough( date purchase_day , int location_1 , int location_2 , int purchase_ticket ) ;

    void print_travel( date purchase_day , int location_1 , int location_2 ) ; // purchase_day 出发 从 location_1 -> location_2 的信息 泛型 print

    void print_train( date query_day ) ; // todo unreleased_train 采用这种方法 print

    void combined_print_train( date query_day , day_train &temp_day_train ) ;

    int get_price( int location_1 , int location_2 ) ;

    int get_time( int location_1 , int location_2 ) ;

    date get_date_index( int location , date purchase_day ) ;

//    int get_max_available_ticket( date purchase_day , int location_1 , int location_2 ) ;

    friend ostream &operator<<( ostream &os , const train &temp_train ) ;

};

class IndexKey
{
public:
    char real_key[42] = {0} ;
public:

    IndexKey(){} ;

    IndexKey( string &input_str ) ;

    IndexKey( const char *input_str ) ;

    IndexKey( user &input_user ) ;

    IndexKey( train &input_train ) ;

    bool operator<( const IndexKey &other_key ) const ;

    bool operator==( const IndexKey &other_key ) const ;

    bool operator>( const IndexKey &other_key ) const ;

    bool operator>=( const IndexKey &other_key ) const ;

    bool operator<=( const IndexKey &other_key ) const ;

    friend ostream &operator<<( ostream &os , const IndexKey &temp_key ) ;

};

class ticket_deal
{
    friend class my_system ;

private:
    char user_name[21] = {0} ;
    char trainID[21] = {0} ;
    char from_location[42] = {0} ;
    char to_location[42] = {0} ;
    int price = 0 , ticket_num = 0 ;
    int int_location_1 = 0 , int_location_2 = 0 ;
//    int deal_sequence = 0 ;  // 用 deal_sum 值
    int deal_priority = 0 ; // todo 用 deal 总量维护
//    bool isWaiting = false ;
    date departure_time , arrival_time , train_set_off ; // todo 发车时间
    ticket_status deal_status ;

    // todo 添加发车时间

    // todo 添加 int_location_1 int_location_2 不用去查车

public: // todo 写一个 modify( train & , int location_1 , int location_2 , date )

    ticket_deal(){}

    ticket_deal( para &input_para ) ; // 仅仅输入基本信息

    ticket_deal &operator= ( const ticket_deal &other ) ;

    bool operator<( const ticket_deal &other_deal ) const ;

    bool operator==( const ticket_deal &other_deal ) const ;

    bool operator>( const ticket_deal &other_deal ) const ;

    void ticket_modify( train &temp_train , int location_1 , int location_2 , date purchase_day ) ;

    void change_status( ticket_status temp_status ) ;

//    void modify_waiting( bool need_to_wait ) ;

//    void modify_sequence( int user_sequence ) ; // 顾客的第几个 deal

    void modify_priority( int train_priority ) ; // 等待队列的第几个 deal

    void modify_time( date from_time , date to_time ) ;

    void modify_price( int deal_price ) ;

    void print_deal() ;

    friend ostream &operator<<( ostream &os , const ticket_deal &temp_deal ) ;

};

// todo day_train 类


#endif //TICKETSYSTEM_2021_MAIN_ALL_HEADER_H
