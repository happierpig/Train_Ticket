#include "Engine.h"

#ifdef my_debug

extern ofstream core_file ;

extern bool stop_core ;

#endif

bool ride::operator<(const ride &other) const
{
    if ( real_type == money ){
        if ( money_cost == other.money_cost ){
            return trainID < other.trainID ;
        }else return money_cost < other.money_cost ;
    }
    else if ( time_cost == other.time_cost ){
        return trainID < other.trainID ;
    }else return time_cost < other.time_cost ;
}

void ride::print_ride()
{
    date set_off_date = ride_purchase_day , arrive_in_date = ride_purchase_day + time_cost ;
    cout << trainID << " " << from_location << " " ;
    set_off_date.print_date() ;
    cout << " -> " << to_location << " " ;
    arrive_in_date.print_date() ;
    cout << " " << money_cost << " " << ride_max_available_ticket << endl ;
}

ride::ride(train &temp_train, int temp_location_1, int temp_location_2, date purchase_day)
{
    location_1 = temp_location_1 ;
    location_2 = temp_location_2 ;
    trainID = temp_train.trainID ;
    from_location = temp_train.all_station[temp_location_1] ;
    to_location = temp_train.all_station[temp_location_2] ;
    money_cost = temp_train.get_price(location_1,location_2) ;
    time_cost = temp_train.get_time(location_1,location_2) ;
    ride_purchase_day = purchase_day ;
    ride_purchase_day.get_other_time(temp_train.all_set_off[location_1]) ;
    ride_max_available_ticket = temp_train.get_max_available_ticket(purchase_day,location_1,location_2) ;
}

void ride::ride_modify( train &temp_train , int temp_location_1 , int temp_location_2 , date purchase_day )
{
    ride temp_ride(temp_train,temp_location_1,temp_location_2,purchase_day) ;
    *this = temp_ride ;
}


my_system::my_system() : user_tree(string(USER_FILE)) , train_tree(string(TRAIN_FILE)) , user_deal_tree(string(DEAL_FILE)) ,
                         location_train_tree(string(LOCATION_FILE)) , waiting_tree(string(WAITING_LIST_FILE)) // 委托构造所有的树
{

}

bool my_system::check_priority(user &c_user, user &u_user)
{
    if ( c_user.privilege > u_user.privilege || c_user == u_user ) return true ;
    return false ;
}

bool my_system::check_login(string &c_user_name){ return log_in_user.count(c_user_name) ; }

void my_system::success() { cout << 0 << endl ; }

void my_system::fail( string err_inf  ) {
    cout << -1 << endl ;

#ifdef my_debug
    cerr << command_stream.str() << endl ;
    if ( !err_inf.empty() ){
        cerr << err_inf << endl ;
    }
#endif
}

void my_system::user_update(user &u_user)
{
    IndexKey user_key(u_user) ;
    if( !user_tree.erase(user_key,u_user ) ) {}
//        cerr << "no such user" << endl  ;
    user_tree.insert(user_key,u_user) ;
}

void my_system::train_update(train &t_train)
{
    IndexKey train_key(t_train) ;
    train_tree.erase(train_key,t_train) ;
    train_tree.insert(train_key,t_train) ;
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
//    cerr << "repeated user" << endl ;
    return false ;
}

void my_system::add_user()
{
    para temp_para(command_stream) ;
    user temp_user(temp_para) ;
    if ( !user_tree.empty() && (!check_login(temp_para.c) || !check_priority(log_in_user[temp_para.c],temp_user) || !no_repeated_user(temp_user)) ){ fail() ; return ; }
    if ( user_tree.empty() ) temp_user.privilege = 10 ;
    IndexKey user_key(temp_para.u) ;
    user_tree.insert(user_key,temp_user) ;
    success() ;
}

void my_system::login()
{
    para temp_para(command_stream) ;
    IndexKey user_key(temp_para.u) ;
    if ( log_in_user.count(temp_para.u) ){ fail() ; return ; } // 已登录不可再登录
    vector<user> ans_vec ;
    user_tree.find(user_key,ans_vec) ;
    if ( ans_vec.empty() || !ans_vec[0].right_password(temp_para.p) ) { fail(); return ; }
    log_in_user[temp_para.u] = ans_vec[0] ;
    success() ;
}

void my_system::logout()
{
    para temp_para( command_stream ) ;
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

void my_system::modify_profile() // todo 用 string -> int 存 用户名 -> priority
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
    if ( check_login(temp_para.u) ){
        log_in_user[temp_para.u] = ans_vec[0] ;
    }
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
    train_tree.insert(train_key,temp_train) ;
    success() ;
}

void my_system::release_train()
{
    para temp_para(command_stream) ;
    IndexKey train_key(temp_para.i) ;
    vector<train> ans_vec ;
    train_tree.find(train_key,ans_vec) ;
    if ( ans_vec.empty() || ans_vec[0].is_released() ) { fail() ; return ; }
    ans_vec[0].release_train() ; // 修改成 release 状态
    for ( int i = 1 ; i <= ans_vec[0].station_num ; i++ ){
        string temp_location(ans_vec[0].all_station[i]) ;
        IndexKey location_key(temp_location) ;
        location_train_tree.insert(location_key,train_key) ;
    }
    if ( !train_tree.erase(train_key,ans_vec[0]) ){
//        cerr << "release erase fail" << endl ;
    } ;
    train_tree.insert(train_key,ans_vec[0]) ;
    success() ;
}

void my_system::query_train()
{
    para temp_para(command_stream) ;
    IndexKey train_key(temp_para.i) ;
    vector<train> ans_vec ;
    train_tree.find(train_key,ans_vec) ;
    date temp_date(temp_para.d) ;
    if ( ans_vec.empty() || !ans_vec[0].in_sale(temp_date,1) ) { fail() ; return ; }
    ans_vec[0].print_train(date(temp_date)) ;
}

void my_system::delete_train()
{
    para temp_para(command_stream) ;
    IndexKey train_key(temp_para.i) ;
    vector<train> ans_vec ;
    train_tree.find(train_key,ans_vec) ;
    if ( ans_vec.empty() || ans_vec[0].is_released() ) { fail() ; return ; }
    train_tree.erase(train_key,ans_vec[0]) ; // todo 没有一次删完这种操作？
    success() ;
}

//void my_system::make_ride(vector<train> &from_train, vector<train> &to_train, string &from_location, string &to_location , vector<pair<ride,train>> &ans_vec )
//{
//    bool find_the_train = false ; // todo 如何判断无车 finish
//    for ( int i = 0 ; i < from_train.size() ; i++ ){
//        for ( int j = 0 ; j < to_train.size() ; j++ ){
//            if ( strcmp(from_train[i].trainID,to_train[j].trainID) == 0 ){
//                int location_1 = from_train[i].get_location(from_location) , location_2 = to_train[j].get_location(to_location) ; // todo 在 make_ride 内处理 in_sale
//                if ( location_1 >= location_2 ) break ; // 直接找下一辆 from_train
//                ride temp_ride ; // todo 此处 调用 ride( train & , location_1  , location_2 , date )
//                temp_ride.location_1 = location_1 ;
//                temp_ride.location_2 = location_2 ;
//                temp_ride.to_location = to_location ;
//                temp_ride.from_location = from_location ;
//                temp_ride.time_cost = from_train[i].get_time(location_1,location_2);
//                temp_ride.money_cost = from_train[i].get_price(location_1,location_2) ;
//                temp_ride.trainID = from_train[i].trainID ;
//                ans_vec.push_back({temp_ride,from_train[i]});
//                break ;
//            }
//        }
//    }
//}

void my_system::make_ride( string &from_location , string &to_location , vector<ride> &ans_vec , vector<IndexKey> &all_train_key , date purchase_day )
{
    vector<train> train_vec ;
    for ( int i = 0 ; i < all_train_key.size() ; i++ ){
        train_vec.clear() ;
        train_tree.find(all_train_key[i],train_vec) ;
        int int_location_1 = train_vec[0].get_location(from_location) , int_location_2 = train_vec[0].get_location(to_location) ;
        if ( int_location_1 >= int_location_2 ) continue ;
        if ( !train_vec[0].in_sale(purchase_day,int_location_1) ) continue ;
        ride temp_ride(train_vec[0],int_location_1,int_location_2,purchase_day) ;
        ans_vec.push_back(temp_ride) ;
    }
}

void my_system::query_ticket() // todo 在找 train_key 的时候就排除不重合车辆 query_ticket too slow
{
    para temp_para(command_stream) ;
    IndexKey from_location_key(temp_para.s) , to_location_key(temp_para.t) ;
    vector<train> from_train , to_train , temp_vec ;
    vector<IndexKey> from_train_key , to_train_key , all_train_key ;
    date purchase_day(temp_para.d) ; // todo  当天的最后一刻在 sale_begin 之后 ， 当天的第一刻在 sale_begin 之前
    location_train_tree.find(from_location_key,from_train_key) ;
    location_train_tree.find(to_location_key,to_train_key) ; // todo 去除所有非重复 Key

    for ( int i = 0 ; i < from_train_key.size() ; i++ ){
        for ( int j = 0 ; j < to_train_key.size() ; j++ ){
            if ( from_train_key[i] == to_train_key[j] ){
                all_train_key.push_back(from_train_key[i]) ;
                break ;
            }
        }
    }

//    for ( int i = 0 ; i < from_train_key.size() ; i++ ){
//        train_tree.find(from_train_key[i],temp_vec) ;
//        if ( temp_vec[0].in_sale(purchase_day,temp_vec[0].get_location(temp_para.s)) ) // todo 拿到车子的时候传引用 避免构造函数
//            from_train.push_back(temp_vec[0]) ; // 不在发售日期不放入
//        temp_vec.clear() ;
//    }
//    for ( int i = 0 ; i < to_train_key.size() ; i++ ){
//        train_tree.find(to_train_key[i],temp_vec) ;
//        to_train.push_back(temp_vec[0]) ;
//        temp_vec.clear() ;
//    } // todo 重构后小心 反向位移未察觉
    vector<ride> available_ride ; // todo 判无车
    make_ride(temp_para.s,temp_para.t,available_ride,all_train_key,purchase_day) ;
    if ( temp_para.p == "cost" )
        for ( auto it = available_ride.begin() ; it != available_ride.end() ; it++){ it->real_type = money ; }
    sort(available_ride.begin(),available_ride.end(),less<ride>()) ;
    if ( available_ride.empty() ) { success() ; return ; } // 无车
    cout << available_ride.size() << endl ;
    for ( int i = 0 ; i < available_ride.size() ; i++ ){
//        available_ride[i].second.print_travel(purchase_day,available_ride[i].first.location_1,available_ride[i].first.location_2) ;
//        cout << " " << available_ride[i].second.get_max_available_ticket(purchase_day,available_ride[i].first.location_1,available_ride[i].first.location_2) << endl ;
        // todo trainID 字典序
        available_ride[i].print_ride() ;
    }
}

void my_system::query_transfer()
{
    para temp_para(command_stream) ;
    IndexKey from_location_key(temp_para.s) ;
    vector<train> train_vec_1 , train_vec_2 ;
    vector<IndexKey> train_key_1 , train_key_2 ;
    ride ans_ride_1 , ans_ride_2 , temp_ride_1 , temp_ride_2 ;
//    train ans_train_1 , ans_train_2 ;
    int ans_cost = MAX_MONEY_COST ; // 取小值
    date ans_purchase_1 , ans_purchase_2 ;
    if ( temp_para.p == "cost" ) { ans_ride_1.real_type = ans_ride_2.real_type = temp_ride_1.real_type = temp_ride_2.real_type = money ; } // todo 直接对后面内容比较
    location_train_tree.find(from_location_key,train_key_1) ;
    date purchase_day(temp_para.d) ;
    string mid_location ;
//    for ( int i = 0 ; i < train_key.size() ; i++ ){
//        train_tree.find(train_key[i],temp_vec) ;
//        if ( temp_vec[0].in_sale(purchase_day,temp_vec[0].get_location(temp_para.s)) ) from_train.push_back(temp_vec[0]) ;
//        temp_vec.clear() ;
//    } // 收集所有出发站的 train // todo temp_date 变量名混乱 finish
//    train_key.clear() ;

    for ( int i = 0 ; i < train_key_1.size() ; i++ ){
        train_vec_1.clear() ;
        train_tree.find(train_key_1[i],train_vec_1) ;
        int start_point = train_vec_1[0].get_location(temp_para.s) ;
        if ( !train_vec_1[0].in_sale(purchase_day,start_point) ) continue ; // todo 第一辆车判日期
        purchase_day.get_other_time(train_vec_1[0].all_set_off[start_point]) ;
        for ( int j = start_point + 1 ; j <= train_vec_1[0].station_num ; j++ ){
            mid_location = train_vec_1[0].all_station[j] ;
            int first_mid_point = j ; // 第一辆车下车位置
            IndexKey mid_location_key(mid_location) ;
            train_key_2.clear() ;
            location_train_tree.find(mid_location_key,train_key_2) ;
            for ( int k = 0 ; k < train_key_2.size() ; k++ ){ // todo 注意隔天发车和非发车日问题
                train_vec_2.clear() ;
                if ( train_key_2[k] == train_key_1[i] ) continue ; // 非同一辆车
                train_tree.find(train_key_2[k],train_vec_2) ;
                int second_mid_point = train_vec_2[0].get_location(mid_location) , end_point = train_vec_2[0].get_location(temp_para.t) ;
                if ( second_mid_point >= end_point ){
                    continue ; // 不到达目的地
                }
                // 判时间
                date mid_arrive_in_day = purchase_day + train_vec_1[0].get_time(start_point,first_mid_point) ;
                date mid_set_off_day = mid_arrive_in_day ;
                if ( !train_vec_2[0].can_take_in_time(mid_set_off_day,second_mid_point ) ) { // todo 传引用变成出发时间
                    continue ;
                }
                temp_ride_1.ride_modify(train_vec_1[0],start_point,first_mid_point,purchase_day) ;
                temp_ride_2.ride_modify(train_vec_2[0],second_mid_point,end_point,mid_set_off_day) ;
                if ( temp_para.p != "cost" ){
                    int temp_ans = mid_set_off_day - purchase_day + temp_ride_2.time_cost ;
                    if ( temp_ans < ans_cost || ( temp_ans == ans_cost && temp_ride_1.time_cost < ans_ride_1.time_cost ) ){
                        ans_ride_1 = temp_ride_1 ;
                        ans_ride_2 = temp_ride_2 ;
                        ans_cost = temp_ans ;
                    }
                }else{
                    int temp_ans = temp_ride_1.money_cost + temp_ride_2.money_cost ;
                    if ( temp_ans < ans_cost || ( temp_ans == ans_cost && temp_ride_1.time_cost < ans_ride_1.time_cost ) ){
                        ans_ride_1 = temp_ride_1 ;
                        ans_ride_2 = temp_ride_2 ;
                        ans_cost = temp_ans ;
                    }
                }
            }
        }
    }
    if ( ans_cost == MAX_MONEY_COST ) { success() ; return ; }
    ans_ride_1.print_ride() ;
    ans_ride_2.print_ride() ;

//    for ( int i = 0 ; i < train_key_1.size() ; i++ ){ // 搜索所有始发车 // todo 直接找到目标即可
////        train train_1 = from_train[i] ;
//        train_vec_1.clear() ;
//        train_tree.find(train_key_1[i],train_vec_1) ;
//        temp_ride_1.trainID = train_1.trainID ;
//        int start_point = train_1.get_location(temp_para.s) ;
//        temp_ride_1.location_1 = start_point ;
//        temp_ride_1.from_location = temp_para.s ;
//        for ( int j = start_point + 1 ; j <= train_1.station_num ; j++ ){ // 找车站1 保证顺序出发
//            mid_location = train_1.all_station[j] ;
//            if ( mid_location == temp_para.t ) continue ; // 必须进行一次换乘 // todo 不知道需不需要保留
//            temp_ride_1.to_location = mid_location ;
//            temp_ride_1.location_2 = j ;
//            temp_ride_1.money_cost = train_1.get_price(temp_ride_1.location_1,temp_ride_1.location_2) ;
//            temp_ride_1.time_cost = train_1.get_time(temp_ride_1.location_1,temp_ride_1.location_2) ;
//            date first_station_set_off = purchase_day ; // 第一站发车时间
//            first_station_set_off.get_other_time(train_1.all_set_off[temp_ride_1.location_1]) ; // temp_date 用来找接下来的发车时间 获得真正的出发时间
//            purchase_day = first_station_set_off ; // 更新 purchase_day
//            date second_station_arrive_in = first_station_set_off + temp_ride_1.time_cost ; // 第一站到站时间 第一站出发时间
//            IndexKey mid_location_key(mid_location) ;
//            location_train_tree.find(mid_location_key,train_key) ; // 去中转站找换乘列车
//            temp_ride_2.to_location = temp_para.t ;
//            temp_ride_2.from_location = mid_location ;
//            for ( int k = 0 ; k < train_key.size() ; k++ ) {
//                if ( strcmp(train_key[k].real_key,train_1.trainID) == 0 ) continue ;
//                train_tree.find(train_key[k],temp_vec) ; // 找到目标车辆
////                train train_2 = temp_vec[0] ;
//                temp_vec.clear() ;
//                int transfer_location = train_2.get_location(mid_location) , final_location = train_2.get_location(temp_para.t) ;
//                if ( final_location <= transfer_location )  continue;  // 未找到目标站 顺序错误 不在销售日期 todo 隔天发车怎么办
//                date second_station_set_off = second_station_arrive_in ; // 预存一下到站日期 next_date 第一站到达时间 temp_date 第二站出发时间
//                second_station_set_off.get_other_time(train_2.all_set_off[transfer_location]) ; // 第二站出发时间
//                if ( second_station_set_off < second_station_arrive_in ) second_station_set_off.add_day() ;  // todo 坐第二天的火车 (要算中间的时间) , 此时 temp_date 为换乘车辆的发车日期 或坐 sale_begin 当天的车 第二站出发时间
//                if ( second_station_set_off > train_2.sale_end + train_2.all_departure[final_location] )  continue; // todo 无论中间隔多久都可以上车 判 sale_begin sale_end
//                if ( !train_2.in_sale(second_station_set_off,transfer_location) ){ // 到达的日子在发车之前 , 坐 sale_begin 当天的车
//                    second_station_set_off = train_2.all_set_off[transfer_location] ; // 第二站出发时间
//                }
//                temp_ride_2.trainID = train_2.trainID ;
//                temp_ride_2.location_1 = transfer_location ;
//                temp_ride_2.location_2 = final_location ;
//                temp_ride_2.money_cost = train_2.get_price(temp_ride_2.location_1,temp_ride_2.location_2) ;
//                temp_ride_2.time_cost = train_2.get_time(temp_ride_2.location_1,temp_ride_2.location_2) ;
//                if ( temp_ride_1.real_type == mytime ){ // -p time
//                    int temp_ans = second_station_set_off - purchase_day + temp_ride_2.time_cost ; // 此处 temp_date 是 第二站发车时间
//                    if ( temp_ans < ans_cost ){
//                        ans_cost = temp_ans ;
//                        ans_ride_1 = temp_ride_1 ;
//                        ans_ride_2 = temp_ride_2 ;
//                        ans_train_1 = train_1 ;
//                        ans_train_2 = train_2 ;
//                        ans_purchase_1 = purchase_day ;
//                        ans_purchase_2 = second_station_set_off ; // 第二站发车时间
//                    }
//                }else{ // -p cost
//                    int temp_ans = temp_ride_1.money_cost + temp_ride_2.money_cost ;
//                    if ( temp_ans < ans_cost ){
//                        ans_cost = temp_ans ;
//                        ans_ride_1 = temp_ride_1 ;
//                        ans_ride_2 = temp_ride_2 ;
//                        ans_train_1 = train_1 ;
//                        ans_train_2 = train_2 ;
//                        ans_purchase_1 = purchase_day ;
//                        ans_purchase_2 = second_station_set_off ; // 第二站发车时间
//                    }
//                }
//            }
//            train_key.clear() ; // 清除中途站的换乘车辆
//        }
//    }
//    if ( ans_cost == MAX_MONEY_COST ) { success() ; return ; } // 实际上是 fail
//    ans_train_1.print_travel(ans_purchase_1,ans_ride_1.location_1,ans_ride_1.location_2) ;
//    cout << " " << ans_train_1.get_max_available_ticket(ans_purchase_1,ans_ride_1.location_1,ans_ride_1.location_2) << endl ;
//    ans_train_2.print_travel(ans_purchase_2,ans_ride_2.location_1,ans_ride_2.location_2) ;
//    cout << " " << ans_train_2.get_max_available_ticket(ans_purchase_2,ans_ride_2.location_1,ans_ride_2.location_2) << endl ;
}

void my_system::buy_ticket()
{
    para temp_para(command_stream) ;
    if ( !check_login(temp_para.u) ) { fail() ; return ; }
    IndexKey train_key(temp_para.i) , user_key(temp_para.u) ;
    vector<train> ans_vec  ; vector<user> user_vec ;
    train_tree.find(train_key,ans_vec) ;
    user_tree.find(user_key,user_vec) ;
    date purchase_day(temp_para.d) ; // todo 将 location 计算一步到位
    if ( ans_vec.empty() || !ans_vec[0].is_released() || ans_vec[0].get_location(temp_para.f) == -1 || ans_vec[0].get_location(temp_para.t) == -1 || !ans_vec[0].in_sale(purchase_day,ans_vec[0].get_location(temp_para.f)) )
    { fail() ; return ; } // todo 将 empty 单独拿出来判断
    ticket_deal temp_deal(temp_para) ;
    int location_1 = ans_vec[0].get_location(temp_para.f) , location_2 = ans_vec[0].get_location(temp_para.t) ;
    int available_ticket_num = ans_vec[0].get_max_available_ticket(purchase_day,location_1,location_2) ; // 最大购买数
    temp_deal.departure_time.get_other_time(ans_vec[0].all_set_off[location_1]) ; // todo 将 temp_deal 和 &train 一起修补 deal 信息
    temp_deal.arrival_time = temp_deal.departure_time + ( ans_vec[0].get_time(location_1,location_2) ) ;
    temp_deal.deal_sequence = user_vec[0].deal_sum + 1 ;
    temp_deal.deal_priority = ans_vec[0].waiting_length + 1 ;
    temp_deal.price = ans_vec[0].get_price(location_1,location_2) ;
    if ( temp_deal.ticket_num > ans_vec[0].seat_num || (available_ticket_num < temp_deal.ticket_num && temp_para.q != "true") ){ fail() ; return ; } // 不愿意等 或 购买过多
    if ( available_ticket_num >= temp_deal.ticket_num ){
        ans_vec[0].ticket_decrease(purchase_day,location_1,location_2,temp_deal.ticket_num) ;
        temp_deal.deal_status = succeed ;
        user_vec[0].change_deal() ;
        deal_update(temp_deal) ; // todo deal_update 改为 insert
        train_update(ans_vec[0]) ;
        user_update(user_vec[0]) ;
        cout << temp_deal.price * temp_deal.ticket_num << endl ;
    }else{
        temp_deal.deal_status = pending ;
        user_vec[0].change_deal() ;
        ans_vec[0].change_waiting_length(1) ;
        deal_update(temp_deal) ; // todo deal_update 改为 insert
        user_update(user_vec[0]) ;
        train_update(ans_vec[0]) ;
        waiting_update(temp_deal) ; // todo waiting_update 改为 insert
        cout << "queue" << endl ;
    }
#ifdef my_debug

    if ( !stop_core && temp_para.i == "LeavesofGrass" && ( temp_para.d == "06-18" || temp_para.d == "06-19" ) ){
        core_file << "buy_ticket -u I_am_the_admin -i LeavesofGrass -d " << temp_para.d << " -n " << temp_para.n << " -f " << temp_para.f << " -t " << temp_para.t ;
        if ( !temp_para.q.empty() ) core_file << " -q " << temp_para.q ;
        core_file << endl ;
        if ( command_stream.str() == "buy_ticket -u Matoimaru -i LeavesofGrass -d 06-19 -n 3691 -f 湖南省张家界市 -t 浙江省湖州市 -q true" ) stop_core = true ;
    }

#endif
}

void my_system::query_order()
{
    para temp_para(command_stream) ;
//    if ( temp_para.u == "Matoimaru" ){
//        cerr << "debug: check " << endl ;
//    }
    if ( !check_login(temp_para.u) ) { fail() ; return ; } // todo 多个订单只输出一个 buy 插入失败？ update_deal 的问题？
    IndexKey user_key(temp_para.u) ;
    vector<ticket_deal> ans_vec ;
    user_deal_tree.find(user_key,ans_vec) ;
    cout << ans_vec.size() << endl ;
    sort(ans_vec.begin(),ans_vec.end(),less<ticket_deal>()) ;
    for ( int i = ans_vec.size() - 1 ; i >= 0 ; i-- ){
        ans_vec[i].print_deal() ;
    }
}

void my_system::refund_ticket() // todo line 265 refund 失败 finish
{
    para temp_para(command_stream) ;
    if ( !check_login(temp_para.u) ) { fail("not log in") ; return ; } // 没有登录
    int target_deal = 1 ;
    if ( !temp_para.n.empty() ){  // 参数 n 为 返回的第 i 张订单
        stringstream change_stream ;
        change_stream << temp_para.n ;
        change_stream >> target_deal ;
    }
    IndexKey user_key(temp_para.u) ;
    vector<ticket_deal> ans_vec , waiting_vec ;
    user_deal_tree.find(user_key,ans_vec) ;
    sort(ans_vec.begin(),ans_vec.end(),less<ticket_deal>()) ;
    if ( target_deal > ans_vec.size() ) { fail("no so many deals") ; return ; } // 没有那么多订单
    ticket_deal temp_deal = ans_vec[ans_vec.size()-target_deal] ;
    if ( temp_deal.deal_status == refunded ) { success() ; return ; } // 直接成功
    string train_str = temp_deal.trainID , from_location = temp_deal.from_location , to_location = temp_deal.to_location ; // 修改 waiting_list \ train \ deal
    IndexKey train_key(train_str) ;
    if ( temp_deal.deal_status == pending ){ // todo 判 queue 状态
        temp_deal.deal_status = refunded ;
        deal_update(temp_deal) ;
        temp_deal.isWaiting = true ; // 从候补队列删除
        waiting_tree.erase(train_key,temp_deal) ;
        success() ;
        return ;
    }
    vector<train> train_vec ;
    train_tree.find(train_key,train_vec) ;
    train_vec[0].ticket_increase(temp_deal.departure_time,train_vec[0].get_location(from_location),train_vec[0].get_location(to_location),temp_deal.ticket_num) ; // 把车票还回去
    waiting_tree.find(train_key,waiting_vec) ;
    sort(waiting_vec.begin(),waiting_vec.end(),less<ticket_deal>()) ;
    for ( int i = 0 ; i < waiting_vec.size() ; i++ ){ // todo 要把 waiting_vec 排序来比较先后 ?
        ticket_deal waiting_deal = waiting_vec[i] ;
        from_location = waiting_vec[i].from_location ;
        to_location = waiting_vec[i].to_location ;
        int location_1 = train_vec[0].get_location(from_location) , location_2 = train_vec[0].get_location(to_location) ; // todo 判当天票
        int available_tickets = train_vec[0].get_max_available_ticket(waiting_deal.departure_time,location_1,location_2) ;
        if ( available_tickets >= waiting_deal.ticket_num ){
//            if ( waiting_deal.price == 959 && waiting_deal.ticket_num == 3691 ){
//                cerr << "debug: check" << endl ;
//            }
            train_vec[0].ticket_decrease(waiting_deal.departure_time,location_1,location_2,waiting_deal.ticket_num) ; // 把区间票数耗光
            waiting_tree.erase(train_key,waiting_deal) ; // 删掉等待队列内容
            waiting_deal.deal_status = succeed ;
            waiting_deal.isWaiting = false ;
            deal_update(waiting_deal) ;
        }
    }
    train_update(train_vec[0]) ; // 修改票数信息
    temp_deal.deal_status = refunded ;
    deal_update(temp_deal) ; // todo refund 的顺序按照交易瞬间进行
    success() ;
}

void my_system::EXIT() {
    cout << "bye" << endl ;
    exit(0) ;
}

void my_system::clean()
{
    user_tree.clear() ;
    train_tree.clear() ;
    user_deal_tree.clear() ;
    location_train_tree.clear() ;
    waiting_tree.clear() ;
    log_in_user.clear() ;
    success() ;
}






