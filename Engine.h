

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

    ride( train &temp_train , int temp_location_1 , int temp_location_2 , date purchase_day , day_train &temp_day_train ) ;

    bool operator<( const ride &other ) const ;

    void print_ride() ;

    void ride_modify( train &temp_train , int temp_location_1 , int temp_location_2 , date purchase_day , day_train &temp_day_train ) ;

};

class my_system
{
private:
    //map<user,int> log_in_user ;
    unordered_map<string,int> log_in_user ;
    stringstream command_stream ;
    BPlusTree<IndexKey,int> user_tree ;
    BPlusTree<IndexKey,int,200,300,5743> train_tree ; // todo 存 int -> 节点 pos
    BPlusTree<IndexKey,ticket_deal> user_deal_tree ;
    BPlusTree<IndexKey,pair<int,int>,200,150,5743> location_train_tree ; // todo 存 location -> trainID + location_i 类精简数据
    BPlusTree<pair<IndexKey,date>,ticket_deal,200,100> waiting_tree ; // todo  <trainID,date> -> ticket_deal
    BPlusTree<pair<IndexKey,date>,int,200,100> day_train_tree ;

    // todo read_user read_day_train

    // todo day_train_tree <trainID,date> -> day_train

    // todo waiting_tree 双 key <trainID,date>

    // todo location_train_tree <location> -> <train_pos,location_i>

    // todo disk manage -> train cache

    fstream real_train_file , real_user_file , real_day_train_file ;

    // bool first_create = true ; // 第一次打开系统特判

public:

    my_system() ;

    void Initialize() ; // todo 直接去 B+树 对象中判 size 需要时再写

    // todo 把 train_insert 和 train_update 封装起来
    // todo int train_insert( train &temp_train )
    // todo void train_update( int train_pos , train &temp_train )
    // todo void read_train( int pos , train &temp_train )

    int train_insert( train &temp_train ) ; // todo 返回位置

    void read_train( int train_pos , train &temp_train ) ;

    int day_train_insert( day_train &temp_day_train ) ;

    void read_day_train( int day_train_pos , day_train &temp_day_train ) ;

    int user_insert( user &temp_user ) ;

    void read_user( int user_pos , user &temp_user ) ;

    bool check_priority( string &c_user_name , user &u_user ) ;

    bool check_login( string &c_user_name ) ;

    bool no_repeated_user( user &u_user ) ;

    void success() ;

    void fail( string err_inf = "" ) ; // fail 之后要 return

    void user_update( int user_pos , user &u_user ) ; // todo 吧 erase -> insert 换成 update

    void train_update( int train_pos  ) ;

    void day_train_update( int day_train_pos , day_train &t_day_train ) ;

    void deal_update( ticket_deal &t_deal ) ; // 购买或修改状态调用

    void waiting_update( ticket_deal &t_deal ) ; // 出 queue 或 入 queue 调用

    void waiting_cancel( ticket_deal &t_deal ) ; // todo 功能被 waiting_update 代替

    void process_command( string &all_command ) ;

    void make_ride( string &from_location , string &to_location , vector<ride> &ans_vec , vector<pair<int,pair<int,int>>> &all_train_key , date purchase_day ) ;

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

    void refund_ticket() ; // todo 不去查 train

    void clean() ;

    void EXIT() ;

    // todo debug_show

    void show_train() ;

    void show_user() ;

    void show_location() ;

};

#endif //TICKETSYSTEM_2021_MAIN_ENGINE_H
