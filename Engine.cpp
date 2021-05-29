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

ride::ride(train &temp_train, int temp_location_1, int temp_location_2, date purchase_day , day_train &temp_day_train )
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
    ride_max_available_ticket = temp_day_train.get_max_available_ticket(location_1,location_2) ;
}

void ride::ride_modify( train &temp_train , int temp_location_1 , int temp_location_2 , date purchase_day , day_train &temp_day_train )
{
    ride temp_ride(temp_train,temp_location_1,temp_location_2,purchase_day,temp_day_train) ;
    *this = temp_ride ;
}


my_system::my_system() : user_tree(string(USER_FILE)) , train_tree(string(TRAIN_FILE)) , user_deal_tree(string(DEAL_FILE)) ,
                         location_train_tree(string(LOCATION_FILE)) , waiting_tree(string(WAITING_LIST_FILE)) ,
                         day_train_tree(string(DAY_TRAIN_FILE)) , real_train_file(REAL_TRAIN_FILE,ios::in|ios::out|ios::binary) ,
                         real_day_train_file(REAL_DAY_TRAIN_FILE,ios::in|ios::out|ios::binary),real_user_file(REAL_USER_FILE,ios::in|ios::out|ios::binary) // 委托构造所有的树
{
    if ( user_tree.size() == 0 ){
        ofstream temp_file(REAL_TRAIN_FILE,ios::binary) , temp_user_file(REAL_USER_FILE,ios::binary) , temp_day_train_file(REAL_DAY_TRAIN_FILE,ios::binary) ;
        temp_file.close() ; temp_user_file.close() ; temp_day_train_file.close() ;
        real_train_file.clear() ; real_user_file.clear() ; real_day_train_file.clear() ;
        real_train_file.open(REAL_TRAIN_FILE,ios::in|ios::out|ios::binary) ;
        real_user_file.open(REAL_USER_FILE,ios::in|ios::out|ios::binary) ;
        real_day_train_file.open(REAL_DAY_TRAIN_FILE,ios::in|ios::out|ios::binary) ;
    }
}

int my_system::train_insert( train &temp_train ) // todo 不知道要不要修补文件 clear
{
    real_train_file.seekp(0,ios::end) ;
    int temp_pos = real_train_file.tellp() ;
    real_train_file.write(reinterpret_cast<char*>(&temp_train),sizeof(train)) ;
    return temp_pos ;
}

int my_system::day_train_insert(day_train &temp_day_train)
{
    real_day_train_file.seekp(0,ios::end) ;
    int temp_pos = real_day_train_file.tellp() ;
    real_day_train_file.write(reinterpret_cast<char*>(&temp_day_train),sizeof(day_train)) ;
    return temp_pos ;
}

void my_system::read_train( int train_pos , train &temp_train )
{
    real_train_file.seekg(train_pos,ios::beg) ;
    real_train_file.read(reinterpret_cast<char*>(&temp_train),sizeof(train)) ;
    real_train_file.clear() ;
}

void my_system::read_user(int user_pos, user &temp_user)
{
    real_user_file.seekg(user_pos,ios::beg) ;
    real_user_file.read(reinterpret_cast<char*>(&temp_user),sizeof(user)) ;
    real_user_file.clear() ;
}
void my_system::read_day_train(int day_train_pos, day_train &temp_day_train)
{
    real_day_train_file.seekg(day_train_pos,ios::beg) ;
    real_day_train_file.read(reinterpret_cast<char*>(&temp_day_train),sizeof(day_train)) ;
    real_day_train_file.clear() ;
}

bool my_system::check_priority(string &c_user_name, user &u_user)
{
    string temp_name(u_user.user_name) ;
    if ( log_in_user[c_user_name] > u_user.privilege || c_user_name == temp_name ) return true ;
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

void my_system::user_update( int user_pos , user &u_user)
{
//    IndexKey user_key(u_user) ;
//    user &real_user = user_tree.update(user_key,u_user) ;
//    real_user = u_user ; // 调用一次赋值
    real_user_file.seekp(user_pos,ios::beg) ;
    real_user_file.write(reinterpret_cast<char*>(&u_user),sizeof(user)) ;
}

void my_system::day_train_update(int day_train_pos, day_train &t_day_train)
{
    real_day_train_file.seekp(day_train_pos,ios::beg) ;
    real_day_train_file.write(reinterpret_cast<char*>(&t_day_train),sizeof(day_train)) ;
}

void my_system::train_update( int train_pos  )
{
    real_train_file.seekp(train_pos,ios::beg) ;
    bool temp_true = true ;
    real_train_file.write(reinterpret_cast<char*>(&temp_true),sizeof(bool)) ;
}

void my_system::deal_update(ticket_deal &t_deal)
{
    IndexKey user_deal_key ;
    strcpy( user_deal_key.real_key , t_deal.user_name ) ;
    ticket_deal &real_deal = user_deal_tree.update(user_deal_key,t_deal) ;
    real_deal = t_deal ;
}

void my_system::waiting_update(ticket_deal &t_deal)
{
//    IndexKey waiting_deal_key ;
//    strcpy( waiting_deal_key.real_key , t_deal.trainID ) ;
//    t_deal.modify_waiting(true) ;
//    if ( t_deal.deal_status == pending )
//        waiting_tree.insert(waiting_deal_key,t_deal) ;
//    else waiting_tree.erase(waiting_deal_key,t_deal) ;
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
    vector<int> ans_vec ;
    user_tree.find(user_key,ans_vec) ;
    if ( ans_vec.empty() ) return true ;
    return false ;
}

int my_system::user_insert(user &temp_user)
{
    real_user_file.seekp(0,ios::end) ;
    int temp_pos = real_user_file.tellp() ;
    real_user_file.write(reinterpret_cast<char*>(&temp_user),sizeof(user)) ;
    return temp_pos ;
}

void my_system::add_user()
{
    para temp_para(command_stream) ;
    user temp_user(temp_para) ;
    if ( !user_tree.empty() && (!check_login(temp_para.c) || !check_priority(temp_para.c,temp_user) || !no_repeated_user(temp_user)) ){ fail() ; return ; }
    if ( user_tree.empty() ) temp_user.privilege = 10 ;
    IndexKey user_key(temp_para.u) ;
    user_tree.insert(user_key,user_insert(temp_user)) ;
    success() ;
}

void my_system::login()
{
    para temp_para(command_stream) ;
    IndexKey user_key(temp_para.u) ;
    if ( log_in_user.count(temp_para.u) ){ fail() ; return ; } // 已登录不可再登录
    vector<int> ans_vec ;
    user_tree.find(user_key,ans_vec) ;
    if ( ans_vec.empty() ) { fail(); return ; }
    user temp_user ;
    read_user(ans_vec[0],temp_user) ;
    if ( !temp_user.right_password(temp_para.p) ) { fail() ; return ; }
    log_in_user[temp_para.u] = temp_user.privilege ;
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
    vector<int> ans_vec ;
    user_tree.find(user_key,ans_vec) ;
    if ( ans_vec.empty() ) { fail(); return ; }
    user temp_user ;
    read_user(ans_vec[0],temp_user) ;
    if ( !check_priority(temp_para.c,temp_user) ) { fail() ; return ; }
    temp_user.print_user() ;
}

void my_system::modify_profile()
{
    para temp_para(command_stream) ;
    if ( !check_login(temp_para.c) ) { fail(); return ; }
    IndexKey user_key(temp_para.u) ;
    vector<int> ans_vec ;
    user_tree.find(user_key,ans_vec) ;
    if ( ans_vec.empty() ) { fail(); return ; }
    user temp_user ;
    read_user( ans_vec[0] , temp_user ) ;
    if ( !check_priority(temp_para.c,temp_user) ) { fail() ; return ; }
    if ( !temp_para.p.empty() ) { strcpy(temp_user.password,temp_para.p.c_str()) ; }
    if ( !temp_para.n.empty() ) { strcpy(temp_user.chinese_name,temp_para.n.c_str()) ; }
    if ( !temp_para.m.empty() ) { strcpy(temp_user.mailAddr,temp_para.m.c_str()) ; }
    if ( !temp_para.g.empty() ) { temp_user.privilege = str_to_int(temp_para.g) ; }
    if ( temp_user.privilege >= log_in_user[temp_para.c] && !temp_para.g.empty() ) { fail(); return ; }
    if ( check_login(temp_para.u) ){
        log_in_user[temp_para.u] = temp_user.privilege ;
    }
    user_update(ans_vec[0],temp_user) ;
    temp_user.print_user() ;
}

void my_system::add_train()
{
    para temp_para(command_stream) ;
    IndexKey train_key(temp_para.i) ;
    vector<int> ans_vec ;
    train_tree.find(train_key,ans_vec) ;
    if ( !ans_vec.empty() ) { fail() ; return ; }
    train temp_train(temp_para) ;
    train_tree.insert(train_key, train_insert(temp_train)) ; // key -> train_pos
    success() ;
}

void my_system::release_train() // todo 将 location_train_key -> int
{
    para temp_para(command_stream) ;
    IndexKey train_key(temp_para.i) ;
    vector<int> ans_vec ;
    train_tree.find(train_key,ans_vec) ;
    if ( ans_vec.empty() ) { fail() ; return ; }
    train temp_train ;
    read_train(ans_vec[0],temp_train) ;
    if ( temp_train.is_released() ){ fail() ; return ; }
    temp_train.release_train() ; // 修改成 release 状态
    day_train every_day_train ;
    for ( int i = 0 ; i < MAX_STATION_SUM ; i++ ){
        every_day_train.seat_num[i] = temp_train.seat_num ;
    }
    for ( int i = 1 ; i <= temp_train.station_num ; i++ ){
        string temp_location(temp_train.all_station[i]) ;
        IndexKey location_key(temp_location) ;
        location_train_tree.insert(location_key, {ans_vec[0],i}) ;
    }
    date temp_date = temp_train.sale_begin ;
    temp_date.become_first_minute() ;
    for ( ; temp_date < temp_train.sale_end || temp_date == temp_train.sale_end ; temp_date = temp_date + 1440 ){
        day_train_tree.insert({train_key,temp_date},day_train_insert(every_day_train)) ;
    }
    train_update(ans_vec[0]) ;
    success() ;
}

void my_system::query_train()
{
    para temp_para(command_stream) ;
    IndexKey train_key(temp_para.i) ;
    vector<int> ans_vec ;
    train_tree.find(train_key,ans_vec) ;
    date temp_date(temp_para.d) ;
    train temp_train ;
    if ( ans_vec.empty() ){ fail() ; return ; }
    read_train(ans_vec[0],temp_train) ;
    if ( !temp_train.in_sale(temp_date,1) ) { fail() ; return ; }
    if ( !temp_train.is_released() ) { temp_train.print_train(date(temp_date)); return ; }
    vector<int> day_train_vec ;
    day_train_tree.find({train_key,temp_date},day_train_vec) ;
    day_train temp_day_train ;
    read_day_train(day_train_vec[0],temp_day_train) ;
    temp_train.combined_print_train(temp_date,temp_day_train) ;
}

void my_system::delete_train()
{
    para temp_para(command_stream) ;
    IndexKey train_key(temp_para.i) ;
    vector<int> ans_vec ;
    train_tree.find(train_key,ans_vec) ;
    if ( ans_vec.empty() ) { fail() ; return ; }
    train temp_train ;
    read_train(ans_vec[0],temp_train) ;
    if ( temp_train.is_released() ) { fail() ; return ; }
    train_tree.erase(train_key,ans_vec[0]) ;
    success() ;
}


void my_system::make_ride( string &from_location , string &to_location , vector<ride> &ans_vec , vector<pair<int,pair<int,int>>> &all_train_key , date purchase_day ) // todo 直接查车
{
    train temp_train ; day_train temp_day_train ;
    vector<int> day_train_vec ; // todo purchase_day 并不是发车日期
    date temp_date = purchase_day ;
    for ( int i = 0 ; i < all_train_key.size() ; i++ ){
        read_train(all_train_key[i].first,temp_train) ; // todo purchase_day 347040
        if ( !temp_train.in_sale(purchase_day,all_train_key[i].second.first) ) continue ;
        temp_date = temp_train.get_date_index(all_train_key[i].second.first,purchase_day) ; // todo 将 get_date_index 封装
        day_train_tree.find({IndexKey(temp_train.trainID),temp_date},day_train_vec) ;
        read_day_train(day_train_vec[0],temp_day_train) ;
        ride temp_ride(temp_train,all_train_key[i].second.first,all_train_key[i].second.second,purchase_day,temp_day_train) ;
        ans_vec.push_back(temp_ride) ;
        day_train_vec.clear() ;
    }
}

void my_system::query_ticket() // todo 将所有 train_key 换成 pos
{
    para temp_para(command_stream) ;
    IndexKey from_location_key(temp_para.s) , to_location_key(temp_para.t) ;
    vector<pair<int,int>> from_train_key , to_train_key ;
    vector<pair<int,pair<int,int>>> all_train_key ;
    date purchase_day(temp_para.d) ; // todo  当天的最后一刻在 sale_begin 之后 ， 当天的第一刻在 sale_begin 之前
    location_train_tree.find(from_location_key,from_train_key) ;
    location_train_tree.find(to_location_key,to_train_key) ; // todo 去除所有非重复 Key

    for ( int i = 0 ; i < from_train_key.size() ; i++ ){
        for ( int j = 0 ; j < to_train_key.size() ; j++ ){
            if ( from_train_key[i].first == to_train_key[j].first && from_train_key[i].second < to_train_key[j].second ){
                all_train_key.push_back({from_train_key[i].first,{from_train_key[i].second,to_train_key[j].second}}) ;
                break ;
            }
        }
    }

// todo 重构后小心 反向位移未察觉
    vector<ride> available_ride ; // todo 判无车
    make_ride(temp_para.s,temp_para.t,available_ride,all_train_key,purchase_day) ;
    if ( temp_para.p == "cost" )
        for ( auto it = available_ride.begin() ; it != available_ride.end() ; it++){ it->real_type = money ; }
    sort(available_ride.begin(),available_ride.end(),less<ride>()) ;
    if ( available_ride.empty() ) { success() ; return ; } // 无车
    cout << available_ride.size() << endl ;
    for ( int i = 0 ; i < available_ride.size() ; i++ ){
        // todo trainID 字典序
        available_ride[i].print_ride() ;
    }
}

void my_system::query_transfer() // todo day_train 没有 modify
{
    para temp_para(command_stream) ;
    IndexKey from_location_key(temp_para.s) ;
    train temp_train_1 , temp_train_2 ;
    vector<pair<int,int>> train_key_1 , train_key_2 ;
    ride ans_ride_1 , ans_ride_2 , temp_ride_1 , temp_ride_2 ;
    int ans_cost = MAX_MONEY_COST ; // 取小值
    date ans_purchase_1 , ans_purchase_2 ;
    if ( temp_para.p == "cost" ) { ans_ride_1.real_type = ans_ride_2.real_type = temp_ride_1.real_type = temp_ride_2.real_type = money ; } // todo 直接对后面内容比较
    location_train_tree.find(from_location_key,train_key_1) ;
    date purchase_day(temp_para.d) ;
    string mid_location ;
    for ( int i = 0 ; i < train_key_1.size() ; i++ ){
        read_train(train_key_1[i].first,temp_train_1) ;
        int start_point = train_key_1[i].second ;
        if ( !temp_train_1.in_sale(purchase_day,start_point) ) continue ; // todo 第一辆车判日期
        purchase_day.get_other_time(temp_train_1.all_set_off[start_point]) ;
        for ( int j = start_point + 1 ; j <= temp_train_1.station_num ; j++ ){
            mid_location = temp_train_1.all_station[j] ;
            int first_mid_point = j ; // 第一辆车下车位置
            IndexKey mid_location_key(mid_location) ;
            train_key_2.clear() ;
            location_train_tree.find(mid_location_key,train_key_2) ;
            for ( int k = 0 ; k < train_key_2.size() ; k++ ){ // todo 注意隔天发车和非发车日问题
                if ( train_key_2[k].first == train_key_1[i].first ) continue ; // 非同一辆车
                read_train(train_key_2[k].first,temp_train_2) ;
                int second_mid_point = train_key_2[k].second , end_point = temp_train_2.get_location(temp_para.t) ;
                if ( second_mid_point >= end_point ){
                    continue ; // 不到达目的地
                }
                // 判时间
                date mid_arrive_in_day = purchase_day + temp_train_1.get_time(start_point,first_mid_point) ;
                date mid_set_off_day = mid_arrive_in_day ;
                if ( !temp_train_2.can_take_in_time(mid_set_off_day,second_mid_point ) ) { // todo 传引用变成出发时间
                    continue ;
                }
                vector<int> temp_day_train_vec ; // todo be careful
                day_train temp_day_train_1 , temp_day_train_2 ;
                day_train_tree.find({IndexKey(temp_train_1.trainID),temp_train_1.get_date_index(start_point,purchase_day)},temp_day_train_vec) ;
                read_day_train(temp_day_train_vec[0],temp_day_train_1) ;
                temp_ride_1.ride_modify(temp_train_1,start_point,first_mid_point,purchase_day,temp_day_train_1) ;
                temp_day_train_vec.clear();
                day_train_tree.find({IndexKey(temp_train_2.trainID),temp_train_2.get_date_index(second_mid_point,mid_set_off_day)},temp_day_train_vec) ;
                read_day_train(temp_day_train_vec[0],temp_day_train_2) ;
                temp_ride_2.ride_modify(temp_train_2,second_mid_point,end_point,mid_set_off_day,temp_day_train_2) ;
                temp_day_train_vec.clear() ;
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
}

void my_system::buy_ticket() // todo 不去读 user 了
{
    para temp_para(command_stream) ;
    if ( !check_login(temp_para.u) ) { fail() ; return ; }
    IndexKey train_key(temp_para.i) , user_key(temp_para.u) ;
    vector<int> ans_vec  ;
    train temp_train ;
    train_tree.find(train_key,ans_vec) ;
    date purchase_day(temp_para.d) ; // todo 将 location 计算一步到位
    if ( ans_vec.empty() ) { fail("no such train") ; return ; }
    read_train(ans_vec[0],temp_train) ;
    int location_1 = temp_train.get_location(temp_para.f) , location_2 = temp_train.get_location(temp_para.t) ;
    if ( !temp_train.is_released() || location_1 == -1 || location_2 == -1 || location_1 >= location_2 || !temp_train.in_sale(purchase_day,location_1) ) { fail("invalid train") ; return ; } // todo Mingwin 环境下会出现错误
    ticket_deal temp_deal(temp_para) ;
    vector<int> temp_day_train_vec ; day_train temp_day_train ;
    day_train_tree.find({train_key,temp_train.get_date_index(location_1,purchase_day)},temp_day_train_vec) ;
    read_day_train(temp_day_train_vec[0],temp_day_train) ;
    int max_available_tickets = temp_day_train.get_max_available_ticket(location_1,location_2) ;
    if ( temp_deal.ticket_num > temp_train.seat_num || ( max_available_tickets < temp_deal.ticket_num && temp_para.q != "true" ) ) { fail() ; return ; }
    temp_deal.ticket_modify(temp_train,location_1,location_2,purchase_day) ; // todo 剥离 user
    temp_deal.deal_priority = user_deal_tree.size() ;
    if ( max_available_tickets >= temp_deal.ticket_num ){
        temp_day_train.modify_seat(location_1,location_2,-temp_deal.ticket_num) ;
        temp_deal.deal_status = succeed ;
        cout << ( long long ) temp_deal.price * ( long long ) temp_deal.ticket_num << endl ;
    }else{
        temp_deal.deal_status = pending ;
        cout << "queue" << endl ;
        waiting_tree.insert({train_key,temp_deal.train_set_off},temp_deal) ;
    }
    user_deal_tree.insert(user_key,temp_deal) ; // todo 修改 day_train
    day_train_update(temp_day_train_vec[0],temp_day_train) ;
//    day_train &real_day_train = day_train_tree.update({train_key,temp_train.get_date_index(location_1,purchase_day)},temp_day_train_vec[0]) ;
//    real_day_train = temp_day_train_vec[0] ; // 直接更改

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
    if ( !temp_para.n.empty() ){ target_deal = str_to_int(temp_para.n) ; }
    IndexKey user_key(temp_para.u) ;
    vector<ticket_deal> ans_vec , waiting_vec ;
    user_deal_tree.find(user_key,ans_vec) ;
    sort(ans_vec.begin(),ans_vec.end(),less<ticket_deal>()) ;
    if ( target_deal > ans_vec.size() ) { fail("no so many deals") ; return ; } // 没有那么多订单
    ticket_deal temp_deal = ans_vec[ans_vec.size()-target_deal] ;
    if ( temp_deal.deal_status == refunded ) { fail() ; return ; } // 直接成功
    string train_str = temp_deal.trainID , from_location = temp_deal.from_location , to_location = temp_deal.to_location ; // 修改 waiting_list \ train \ deal
    IndexKey train_key(train_str) ;
    if ( temp_deal.deal_status == pending ){ // todo 判 queue 状态
        temp_deal.deal_status = refunded ;
        deal_update(temp_deal) ;
        waiting_tree.erase({train_key,temp_deal.train_set_off},temp_deal) ;
        success() ;
        return ;
    }
    vector<int> day_train_vec ; day_train temp_day_train ;
    day_train_tree.find({IndexKey(temp_deal.trainID),temp_deal.train_set_off},day_train_vec) ;
    read_day_train(day_train_vec[0],temp_day_train) ;
    temp_day_train.modify_seat(temp_deal.int_location_1,temp_deal.int_location_2,temp_deal.ticket_num) ; // 把车票还回去
    waiting_tree.find({train_key,temp_deal.train_set_off},waiting_vec) ;
    sort(waiting_vec.begin(),waiting_vec.end(),less<ticket_deal>()) ;
    for ( int i = 0 ; i < waiting_vec.size() ; i++ ){ // todo 要把 waiting_vec 排序来比较先后 ?
        int available_tickets = temp_day_train.get_max_available_ticket(waiting_vec[i].int_location_1,waiting_vec[i].int_location_2) ;
        if ( available_tickets >= waiting_vec[i].ticket_num ){
            temp_day_train.modify_seat(waiting_vec[i].int_location_1,waiting_vec[i].int_location_2,-waiting_vec[i].ticket_num) ; // 把区间票数耗光
            waiting_tree.erase({train_key,temp_deal.train_set_off},waiting_vec[i]) ; // 删掉等待队列内容
            waiting_vec[i].deal_status = succeed ;
            deal_update(waiting_vec[i]) ;
        }
    }
    temp_deal.deal_status = refunded ; // todo 修改 day_train 信息
    deal_update(temp_deal) ;
//    day_train &real_day_train = day_train_tree.update({IndexKey(temp_deal.trainID),temp_deal.train_set_off},day_train_vec[0]) ;
//    real_day_train = day_train_vec[0] ;
    day_train_update(day_train_vec[0],temp_day_train) ;
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













