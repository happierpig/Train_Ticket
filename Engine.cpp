#include "Engine.h"

bool ride::operator<(const ride &other) const
{
    if ( real_type == money ) return money_cost < other.money_cost ;
    return time_cost < other.time_cost ;
}


my_system::my_system() : user_tree(string(USER_FILE)) , train_tree(string(TRAIN_FILE)) , user_deal_tree(string(DEAL_FILE)) ,
                         location_train_tree(string(LOCATION_FILE)) , waiting_tree(string(WAITING_LIST_FILE)) // 委托构造所有的树
{

}

bool my_system::check_priority(user &c_user, user &u_user)
{
    if ( c_user.privilege > u_user.privilege || c_user == u_user ) return true ;
    cerr << "low priority" << endl ;
    return false ;
}

bool my_system::check_login(string &c_user_name){ return log_in_user.count(c_user_name) ; }

void my_system::success() { cout << 0 << endl ; }

void my_system::fail() { cout << -1 << endl ; }

void my_system::user_update(user &u_user)
{
    IndexKey user_key(u_user) ;
    if( !user_tree.erase(user_key,u_user ) )
        cerr << "no such user" << endl  ;
    user_tree.insert(user_key,u_user) ;
    // todo debug
    vector<user> check_vec ;
    user_tree.find(user_key,check_vec) ;
    if ( check_vec.empty() ){
        cerr << "user insert fail" << endl ;
    }
}

void my_system::train_update(train &t_train) // todo 出问题 ()
{
    IndexKey train_key(t_train) ;
    if( !train_tree.erase(train_key,t_train) )
        cerr << "no such train" << endl ;
    train_tree.insert(train_key,t_train) ;
    // todo debug
    vector<train> check_vec ;
    train_tree.find(train_key,check_vec) ;
    if ( check_vec.empty() )
        cerr << "train insert fail" << endl ;
}

void my_system::deal_update(ticket_deal &t_deal)
{
    IndexKey user_deal_key ;
    strcpy( user_deal_key.real_key , t_deal.user_name ) ;
    t_deal.modify_waiting(false) ;
    user_deal_tree.erase(user_deal_key,t_deal) ;
    user_deal_tree.insert(user_deal_key,t_deal) ;
}

void my_system::waiting_update(ticket_deal &t_deal)
{
    IndexKey waiting_deal_key ;
    strcpy( waiting_deal_key.real_key , t_deal.trainID ) ;
    t_deal.modify_waiting(true) ;
    if ( t_deal.deal_status == pending )
        waiting_tree.insert(waiting_deal_key,t_deal) ;
    else waiting_tree.erase(waiting_deal_key,t_deal) ;
}

void my_system::process_command(string &all_command)
{
    string real_command ;
    command_stream.str("") ;
    command_stream.clear() ;
    command_stream << all_command ;
    command_stream >> real_command ;
    if ( real_command == "add_user" ) { add_user() ; }
    else if ( real_command == "login" ) { login() ; }
    else if ( real_command == "logout" ) { logout() ; }
    else if ( real_command == "query_profile" ) { query_profile() ; }
    else if ( real_command == "modify_profile" ) { modify_profile() ; }
    else if ( real_command == "add_train" ) { add_train() ; }
    else if ( real_command == "release_train" ) { release_train() ; }
    else if ( real_command == "query_train" ) { query_train() ; }
    else if ( real_command == "delete_train" ) { delete_train() ; }
    else if ( real_command == "query_ticket" ) { query_ticket() ; }
    else if ( real_command == "query_transfer" ) { query_transfer() ; }
    else if ( real_command == "buy_ticket" ) { buy_ticket() ; }
    else if ( real_command == "query_order" ) { query_order() ; }
    else if ( real_command == "refund_ticket" ) { refund_ticket() ; }
    else if ( real_command == "exit" ) { EXIT() ; }
    else if ( real_command == "clean" ) { clean() ; }
}

bool my_system::no_repeated_user(user &u_user)
{
    IndexKey user_key(u_user) ;
    vector<user> ans_vec ;
    user_tree.find(user_key,ans_vec) ;
    if ( ans_vec.empty() ) return true ;
    cerr << "repeated user" << endl ;
    return false ;
}

void my_system::add_user()
{
    para temp_para(command_stream) ;
    user temp_user(temp_para) ;
    if ( !user_tree.empty() && (!check_login(temp_para.c) || !check_priority(log_in_user[temp_para.c],temp_user) || !no_repeated_user(temp_user)) ){ fail() ; return ; }
    if ( user_tree.empty() ) temp_user.privilege = 10 ;
    user_update(temp_user) ;
    success() ;
}

void my_system::login()
{
    para temp_para(command_stream) ;
    IndexKey user_key(temp_para.u) ;
    vector<user> ans_vec ;
    user_tree.find(user_key,ans_vec) ;
    if ( ans_vec.empty() || !ans_vec[0].right_password(temp_para.p) ) { fail(); return ; }
    log_in_user[temp_para.u] = ans_vec[0] ;
    success() ;
}

void my_system::logout()
{
    para temp_para( command_stream ) ;
    IndexKey user_key(temp_para.u) ;
    if ( !log_in_user.count(temp_para.u) ) { fail(); return ; }
    log_in_user.erase(temp_para.u) ;
    success() ;
}

void my_system::query_profile()
{
    para temp_para( command_stream ) ;
    if ( !check_login(temp_para.c) ) { fail(); return ; }
    IndexKey user_key(temp_para.u) ;
    vector<user> ans_vec ;
    user_tree.find(user_key,ans_vec) ;
    if ( ans_vec.empty() || !check_priority(log_in_user[temp_para.c],ans_vec[0]) ) { fail(); return ; }
    ans_vec[0].print_user() ;
}

void my_system::modify_profile()
{
    para temp_para(command_stream) ;
    if ( !check_login(temp_para.c) ) { fail(); return ; }
    IndexKey user_key(temp_para.u) ;
    vector<user> ans_vec ;
    user_tree.find(user_key,ans_vec) ;
    if ( ans_vec.empty() || !check_priority(log_in_user[temp_para.c],ans_vec[0]) ) { fail(); return ; }
    stringstream change_stream ;
    if ( !temp_para.p.empty() )
    { change_stream << temp_para.p ; change_stream >> ans_vec[0].password ; change_stream.str("") ; change_stream.clear() ; }
    if ( !temp_para.n.empty() )
    { change_stream << temp_para.n ; change_stream >> ans_vec[0].chinese_name ; change_stream.str("") ; change_stream.clear() ; }
    if ( !temp_para.m.empty() )
    { change_stream << temp_para.m ; change_stream >> ans_vec[0].mailAddr ; change_stream.str("") ; change_stream.clear() ; }
    if ( !temp_para.g.empty() )
    { change_stream << temp_para.g ; change_stream >> ans_vec[0].privilege ; change_stream.str("") ; change_stream.clear() ; }
    if ( ans_vec[0].privilege >= log_in_user[temp_para.c].privilege ) { fail(); return ; }
    user_update(ans_vec[0]) ;
    ans_vec[0].print_user() ;
}

void my_system::add_train()
{
    para temp_para(command_stream) ;
    IndexKey train_key(temp_para.i) ;
    vector<train> ans_vec ;
    train_tree.find(train_key,ans_vec) ;
    if ( !ans_vec.empty() ) { fail() ; return ; }
    train temp_train(temp_para) ;
//    train_update(temp_train) ;
    train_tree.insert(train_key,temp_train) ;
    // todo debug
    train_tree.find(train_key,ans_vec) ;
    if ( ans_vec.empty() ){
        cerr << "add_train insert fail" << endl ;
    }
    success() ;
}

void my_system::release_train()
{
    para temp_para(command_stream) ;
    IndexKey train_key(temp_para.i) ;
    vector<train> ans_vec ;
    train_tree.find(train_key,ans_vec) ;
    if ( ans_vec.empty() || ans_vec[0].is_released() ) { fail() ; return ; }
    ans_vec[0].release_train() ;
    for ( int i = 1 ; i <= ans_vec[0].station_num ; i++ ){
        string temp_location(ans_vec[0].all_station[i]) ;
        IndexKey location_key(temp_location) ;
        location_train_tree.insert(location_key,train_key) ;
    }
//    train_update(ans_vec[0]) ;
    if ( !train_tree.erase(train_key,ans_vec[0]) ){
        cerr << "release erase fail" << endl ;
    } ;
    train_tree.insert(train_key,ans_vec[0]) ;
    success() ;
    // todo debug
    ans_vec.clear() ;
    train_tree.find(train_key,ans_vec) ;
    if ( ans_vec.empty() ){
        cerr << "release fail" << endl ;
    }
}

void my_system::query_train()
{
    para temp_para(command_stream) ;
    IndexKey train_key(temp_para.i) ;
    vector<train> ans_vec ;
    train_tree.find(train_key,ans_vec) ;
    if ( ans_vec.empty() ) { fail() ; return ; }
    ans_vec[0].print_train(date(temp_para.d)) ;
}

void my_system::delete_train()
{
    para temp_para(command_stream) ;
    IndexKey train_key(temp_para.i) ;
    vector<train> ans_vec ;
    train_tree.find(train_key,ans_vec) ;
    if ( ans_vec.empty() || ans_vec[0].is_released() ) { fail() ; return ; }
    train_tree.erase(train_key,ans_vec[0]) ; // todo 没有一次删完这种操作？
}

void my_system::make_ride(vector<train> &from_train, vector<train> &to_train, string &from_location, string &to_location , vector<pair<ride,train>> &ans_vec )
{
    bool find_the_train = false ;
    for ( int i = 0 ; i < from_train.size() ; i++ ){
        for ( int j = 0 ; j < to_train.size() ; j++ ){
            if ( strcmp(from_train[i].trainID,to_train[j].trainID) == 0 ){
                int location_1 = from_train[i].get_location(from_location) , location_2 = to_train[j].get_location(to_location) ;
                if ( location_1 >= location_2 ) break ; // 直接找下一辆 from_train
                ride temp_ride ;
                temp_ride.location_1 = location_1 ;
                temp_ride.location_2 = location_2 ;
                temp_ride.to_location = to_location ;
                temp_ride.from_location = from_location ;
                temp_ride.time_cost = from_train[i].get_time(location_1,location_2);
                temp_ride.money_cost = from_train[i].get_price(location_1,location_2) ;
                temp_ride.trainID = from_train[i].trainID ;
                ans_vec.push_back({temp_ride,from_train[i]});
                break ;
            }
        }
    }
}

void my_system::query_ticket()
{
    para temp_para(command_stream) ;
    IndexKey from_location_key(temp_para.s) , to_location_key(temp_para.t) ;
    vector<train> from_train , to_train , temp_vec ;
    vector<IndexKey> from_train_key , to_train_key ;
    date purchase_day(temp_para.d) ;
    location_train_tree.find(from_location_key,from_train_key) ;
    location_train_tree.find(to_location_key,to_train_key) ;
    for ( int i = 0 ; i < from_train_key.size() ; i++ ){
        train_tree.find(from_train_key[i],temp_vec) ;
        if ( temp_vec[0].in_sale(purchase_day,temp_vec[0].get_location(temp_para.s)) )
            from_train.push_back(temp_vec[0]) ; // 不在发售日期不放入
        temp_vec.clear() ;
    }
    for ( int i = 0 ; i < to_train_key.size() ; i++ ){
        train_tree.find(to_train_key[i],temp_vec) ;
        if ( temp_vec[0].in_sale(purchase_day,temp_vec[0].get_location(temp_para.t)) )
            to_train.push_back(temp_vec[0]) ;
        temp_vec.clear() ;
    }
    vector<pair<ride,train>> available_ride ; // todo 判无车
    make_ride(from_train,to_train,temp_para.s,temp_para.t,available_ride) ;
    if ( temp_para.p == "cost" ) for ( auto it = available_ride.begin() ; it != available_ride.end() ; it++){ it->first.real_type = money ; }
    sort(available_ride.begin(),available_ride.end(),greater<pair<ride,train>>()) ;
    if ( available_ride.empty() ) { fail() ; return ; }
    cout << available_ride.size() << endl ;
    for ( int i = 0 ; i < available_ride.size() ; i++ ){
        available_ride[i].second.print_travel(purchase_day,available_ride[i].first.location_1,available_ride[i].first.location_2) ;
        cout << " " << available_ride[i].second.get_max_available_ticket(purchase_day,available_ride[i].first.location_1,available_ride[i].first.location_2) << endl ;
    }
}

void my_system::query_transfer()
{
    para temp_para(command_stream) ;
    IndexKey from_location_key(temp_para.s) ;
    vector<train> from_train  , temp_vec ; vector<IndexKey> train_key ;
    ride ans_ride_1 , ans_ride_2 , temp_ride_1 , temp_ride_2 ;
    train ans_train_1 , ans_train_2 ;
    int ans_cost = 10000000 ; // 取小值
    date ans_purchase_1 , ans_purchase_2 ;
    if ( temp_para.p == "cost" ) { ans_ride_1.real_type = ans_ride_2.real_type = temp_ride_1.real_type = temp_ride_2.real_type = money ; }
    location_train_tree.find(from_location_key,train_key) ;
    date purchase_day(temp_para.d) ; string mid_location ;
    for ( int i = 0 ; i < train_key.size() ; i++ ){
        train_tree.find(train_key[i],temp_vec) ;
        if ( temp_vec[0].in_sale(purchase_day,temp_vec[0].get_location(temp_para.s)) ) from_train.push_back(temp_vec[0]) ;
        temp_vec.clear() ;
    } // 收集所有出发站的 train
    train_key.clear() ;
    for ( int i = 0 ; i < from_train.size() ; i++ ){ // 搜索所有始发车
        train train_1 = from_train[i] ;
        temp_ride_1.trainID = train_1.trainID ;
        int start_point = train_1.get_location(temp_para.s) ;
        temp_ride_1.location_1 = start_point ;
        for ( int j = start_point + 1 ; j <= train_1.station_num ; j++ ){ // 找车站1 保证顺序出发
            mid_location = train_1.all_station[j] ;
            if ( mid_location == temp_para.t ) continue ; // 必须进行一次换乘
            temp_ride_1.to_location = mid_location ;
            temp_ride_1.location_2 = j ;
            temp_ride_1.money_cost = train_1.get_price(temp_ride_1.location_1,temp_ride_1.location_2) ;
            temp_ride_1.time_cost = train_1.get_time(temp_ride_1.location_1,temp_ride_1.location_2) ;
            date temp_date = purchase_day ;
            temp_date.get_other_time(train_1.all_set_off[temp_ride_1.location_1]) ; // temp_date 用来找接下来的发车时间 获得真正的出发时间
            purchase_day = temp_date ; // 更新 purchase_day
            temp_date = temp_date + temp_ride_1.time_cost ; // 到站了
            IndexKey mid_location_key(mid_location) ;
            location_train_tree.find(mid_location_key,train_key) ; // 去中转站找换乘列车
            temp_ride_2.to_location = temp_para.t ;
            temp_ride_2.from_location = mid_location ;
            for ( int k = 0 ; k < train_key.size() ; k++ ) {
                train_tree.find(train_key[k],temp_vec) ; // 找到目标车辆
                train train_2 = temp_vec[0] ;
                temp_vec.clear() ;
                int transfer_location = train_2.get_location(mid_location) ;
                if ( train_2.get_location(temp_para.t) <= transfer_location )  continue;  // 未找到目标站 顺序错误 不在销售日期 todo 隔天发车怎么办
                date next_date = temp_date ; // 预存一下到站日期
                temp_date.get_other_time(train_2.all_set_off[transfer_location]) ;
                if ( temp_date < next_date) temp_date = temp_date + 1440 ; // 坐第二天的火车 (要算中间的时间) , 此时 temp_date 为换乘车辆的发车日期
                if ( !train_2.in_sale(temp_date,transfer_location) )  continue;
                temp_ride_2.trainID = train_2.trainID ;
                temp_ride_2.location_1 = transfer_location ;
                temp_ride_2.location_2 = train_2.get_location(temp_para.t) ;
                temp_ride_2.money_cost = train_2.get_price(temp_ride_2.location_1,temp_ride_2.location_2) ;
                temp_ride_2.time_cost = train_2.get_time(temp_ride_2.location_1,temp_ride_2.location_2) ;
                if ( temp_ride_1.real_type == mytime ){ // -p time
                    int temp_ans = temp_date - purchase_day + temp_ride_2.time_cost ; // 总共消耗时间
                    if ( temp_ans < ans_cost ){
                        ans_cost = temp_ans ;
                        ans_ride_1 = temp_ride_1 ;
                        ans_ride_2 = temp_ride_2 ;
                        ans_train_1 = train_1 ;
                        ans_train_2 = train_2 ;
                        ans_purchase_1 = purchase_day ;
                        ans_purchase_2 = temp_date ;
                    }
                }else{ // -p cost
                    int temp_ans = temp_ride_1.money_cost + temp_ride_2.money_cost ;
                    if ( temp_ans < ans_cost ){
                        ans_cost = temp_ans ;
                        ans_ride_1 = temp_ride_1 ;
                        ans_ride_2 = temp_ride_2 ;
                        ans_train_1 = train_1 ;
                        ans_train_2 = train_2 ;
                        ans_purchase_1 = purchase_day ;
                        ans_purchase_2 = temp_date ;
                    }
                }
            }
            train_key.clear() ; // 清除中途站的换乘车辆
        }
    }
    if ( ans_cost == 10000000 ) { success() ; return ; } // 实际上是 fail
    ans_train_1.print_travel(ans_purchase_1,temp_ride_1.location_1,temp_ride_1.location_2) ;
    cout << " " << ans_train_1.get_max_available_ticket(ans_purchase_1,temp_ride_1.location_1,temp_ride_1.location_2) << endl ;
    ans_train_2.print_travel(ans_purchase_2,temp_ride_2.location_1,temp_ride_2.location_2) ;
    cout << " " << ans_train_2.get_max_available_ticket(ans_purchase_2,temp_ride_2.location_1,temp_ride_2.location_2) << endl ;
}

void my_system::buy_ticket()
{
    para temp_para(command_stream) ;
    if ( !check_login(temp_para.u) ) { fail() ; return ; }
    IndexKey train_key(temp_para.i) , user_key(temp_para.u) ;
    vector<train> ans_vec  ; vector<user> user_vec ;
    train_tree.find(train_key,ans_vec) ; // todo 这里ans_vec 找不到
    user_tree.find(user_key,user_vec) ;
    date purchase_day(temp_para.d) ;
    if ( ans_vec.empty() || !ans_vec[0].is_released() || ans_vec[0].get_location(temp_para.f) == -1 || ans_vec[0].get_location(temp_para.t) == -1 || !ans_vec[0].in_sale(purchase_day,ans_vec[0].get_location(temp_para.f)) )
    { fail() ; return ; } // todo 第二次就出事，第三次就消失
    ticket_deal temp_deal(temp_para) ;
    int location_1 = ans_vec[0].get_location(temp_para.f) , location_2 = ans_vec[0].get_location(temp_para.t) ;
    int available_ticket_num = ans_vec[0].get_max_available_ticket(purchase_day,location_1,location_2) ;
    temp_deal.departure_time.get_other_time(ans_vec[0].all_set_off[location_1]) ; // 修补 deal 信息
    temp_deal.arrival_time = temp_deal.departure_time + ( ans_vec[0].get_time(location_1,location_2) ) ;
    temp_deal.deal_sequence = user_vec[0].deal_sum + 1 ;
    temp_deal.deal_priority = ans_vec[0].waiting_length + 1 ;
    temp_deal.price = ans_vec[0].get_price(location_1,location_2) ;
    if ( available_ticket_num > ans_vec[0].seat_num || (available_ticket_num < temp_deal.ticket_num && temp_para.q == "true") ) { fail() ; return ; }
    if ( available_ticket_num >= temp_deal.ticket_num ){
        ans_vec[0].ticket_decrease(purchase_day,location_1,location_2,temp_deal.ticket_num) ;
        temp_deal.deal_status = succeed ;
        user_vec[0].change_deal() ;
        deal_update(temp_deal) ;
        train_update(ans_vec[0]) ;
        user_update(user_vec[0]) ;
        cout << temp_deal.price * temp_deal.ticket_num << endl ;
    }else{
        temp_deal.deal_status = pending ;
        user_vec[0].change_deal() ;
        ans_vec[0].change_waiting_length(1) ;
        deal_update(temp_deal) ;
        user_update(user_vec[0]) ;
        train_update(ans_vec[0]) ;
        waiting_update(temp_deal) ;
        cout << "queue" << endl ;
    }

}

void my_system::query_order()
{
    para temp_para(command_stream) ;
    if ( !check_login(temp_para.u) ) { fail() ; return ; }
    IndexKey user_key(temp_para.u) ;
    vector<ticket_deal> ans_vec ;
    user_deal_tree.find(user_key,ans_vec) ;
    cout << ans_vec.size() << endl ;
    for ( int i = 0 ; i < ans_vec.size() ; i++ ){
        ans_vec[i].print_deal() ;
    }
}

void my_system::refund_ticket()
{
    para temp_para(command_stream) ;
    if ( !check_login(temp_para.u) ) { fail() ; return ; }
    int target_deal = 1 ;
    if ( !temp_para.u.empty() ){
        stringstream change_stream ;
        change_stream << temp_para.u ;
        change_stream >> target_deal ;
    }
    IndexKey user_key(temp_para.u) ;
    vector<ticket_deal> ans_vec , waiting_vec ;
    user_deal_tree.find(user_key,ans_vec) ;
    if ( target_deal > ans_vec.size() ) { fail() ; return ; } // 没有那么多订单
    ticket_deal temp_deal = ans_vec[ans_vec.size()-target_deal] ;
    if ( temp_deal.deal_status != succeed ) { fail() ; return ; } // 并没有购买成功
    string train_str = temp_deal.trainID , from_location = temp_deal.from_location , to_location = temp_deal.to_location ; // 修改 waiting_list \ train \ deal
    IndexKey train_key(train_str) ;
    vector<train> train_vec ;
    train_tree.find(train_key,train_vec) ;
    train_vec[0].ticket_increase(temp_deal.departure_time,train_vec[0].get_location(from_location),train_vec[0].get_location(to_location),temp_deal.ticket_num) ; // 把车票还回去
    waiting_tree.find(train_key,waiting_vec) ;
    for ( int i = 0 ; i < waiting_vec.size() ; i++ ){
        ticket_deal waiting_deal = waiting_vec[i] ;
        from_location = waiting_vec[i].from_location ;
        to_location = waiting_vec[i].to_location ;
        int location_1 = train_vec[0].get_location(from_location) , location_2 = train_vec[0].get_location(to_location) ;
        int available_tickets = train_vec[0].get_max_available_ticket(waiting_deal.departure_time,location_1,location_2) ;
        if ( available_tickets >= waiting_deal.ticket_num ){
            train_vec[0].ticket_decrease(waiting_deal.departure_time,location_1,location_2,waiting_deal.ticket_num) ;
            waiting_tree.erase(train_key,waiting_deal) ;
            waiting_deal.deal_status = succeed ;
            waiting_deal.isWaiting = false ;
            deal_update(waiting_deal) ;
        }
    }
    train_update(train_vec[0]) ; // 修改票数信息
    deal_update(temp_deal) ; // 修改票 refunded 状态
    success() ;
}

void my_system::EXIT() {
    cout << "bye" << endl ;
    exit(0) ;
}

void my_system::clean()
{

}






