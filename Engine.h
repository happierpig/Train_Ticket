

#ifndef TICKETSYSTEM_2021_MAIN_ENGINE_H
#define TICKETSYSTEM_2021_MAIN_ENGINE_H

#include "all_header.h"
using namespace std ;

enum cost_type{ money , mytime };

//#define my_debug

class ride
{
public:
    string from_location , to_location ;
    string trainID ;
    int location_1 = 0  , location_2 = 0 ;
    cost_type real_type = mytime ;
    int money_cost = 0 , time_cost = 0 ;
    date ride_purchase_day ; // 此处 day 是确切时间
    int ride_max_available_ticket = 0 ;

public:

    ride(){} ;

    ride( train &temp_train , int temp_location_1 , int temp_location_2 , date purchase_day ) ;

    bool operator<( const ride &other ) const ;

    void print_ride() ;

    void ride_modify( train &temp_train , int temp_location_1 , int temp_location_2 , date purchase_day ) ;

    // todo 用 ride 实现 print_travel

    // todo 用 train 来 构造 ride

};

class my_system
{
private:
    //map<user,int> log_in_user ;
    map<string,user> log_in_user ; // todo 考虑 unordered_map<string,int> log_in_user 前存用户ID 后存 priority
    stringstream command_stream ;
    BPlusTree<IndexKey,user> user_tree ;
    BPlusTree<IndexKey,train,300,6,31> train_tree ;
    BPlusTree<IndexKey,ticket_deal,300,100> user_deal_tree ; // todo 修改参数信息丢失
    BPlusTree<IndexKey,IndexKey> location_train_tree ;
    BPlusTree<IndexKey,ticket_deal> waiting_tree ;
    // bool first_create = true ; // 第一次打开系统特判

public:

    my_system() ;

    void Initialize() ; // todo 直接去 B+树 对象中判 size 需要时再写

    bool check_priority( user &c_user , user &u_user ) ;

    bool check_login( string &c_user_name ) ;

    bool no_repeated_user( user &u_user ) ;

    void success() ;

    void fail( string err_inf = "" ) ; // fail 之后要 return

    void user_update( user &u_user ) ;

    void train_update( train &t_train ) ;

    void deal_update( ticket_deal &t_deal ) ; // 购买或修改状态调用

    void waiting_update( ticket_deal &t_deal ) ; // 出 queue 或 入 queue 调用

    void waiting_cancel( ticket_deal &t_deal ) ; // todo 功能被 waiting_update 代替

    void process_command( string &all_command ) ;

    void make_ride( string &from_location , string &to_location , vector<ride> &ans_vec , vector<IndexKey> &all_train_key , date purchase_day ) ; // todo 重构

    void add_user() ;

    void login() ;

    void logout() ;

    void query_profile() ;

    void modify_profile() ;

    void add_train() ;

    void release_train() ;

    void query_train() ;

    void delete_train() ;

    void query_ticket() ;

    void query_transfer() ; // todo 第一辆车乘坐时间更少

    void buy_ticket() ;

    void query_order() ;

    void refund_ticket() ;

    void clean() ;

    void EXIT() ;

    // todo debug_show

    void show_train() ;

    void show_user() ;

    void show_location() ;

};

#endif //TICKETSYSTEM_2021_MAIN_ENGINE_H
