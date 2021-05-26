#include "all_header.h"
using namespace std ;

int get_strip( string &temp_str )
{
    int counter = 0 ;
    for ( int i = 0 ; i < temp_str.length() ; i++ ){
        if ( temp_str[i] == '|' ) counter++ ;
    }
    return counter ;
}

int str_to_int( const string &other )
{
    int temp_sum = 0 ;
    for ( int i = 0 ; i < other.size() ; i++ ){
        temp_sum *= 10 ;
        temp_sum += other[i] - '0' ;
    }
    return temp_sum ;
}

void get_split_context( string &input_str , stringstream &temp_stream )
{
    int strip_num = get_strip(input_str) ;
    stringstream func_stream ;
    string temp_str ;
    func_stream << input_str ;
    for ( int i = 0 ; i < strip_num ; i++ ){
        getline(func_stream,temp_str,'|') ;
        temp_stream << temp_str << " " ;
    }
    func_stream >> temp_str ;
    temp_stream << temp_str ;
}

date::date(string &date_str)
{
    int temp_time = 0 , temp_month = 0 , temp_day = 0 ;
    temp_month = date_str[1] - '0' ;
    temp_day = (date_str[3] - '0') * 10 + date_str[4] - '0' ;
    temp_time += all_month_add_up[temp_month] + ( temp_day - 1 ) * 24 * 60 ;
    all_time = temp_time ;
}

date::date(string &date_str, string &time_str) : date(date_str)
{
    int temp_hour = 0 , temp_minute = 0 ;
    temp_hour = ( time_str[0] - '0' ) * 10 + time_str[1] - '0' ;
    temp_minute = ( time_str[3] - '0' ) * 10 + time_str[4] - '0' ;
    all_time += temp_hour * 60 + temp_minute ;
}

void date::add_day(){ all_time += 1440 ; }

void date::add_hour(){ all_time += 60 ; }

void date::add_minute(){ all_time += 1 ; }

date date::operator+( int interval_time ) {
    date temp = *this ;
    temp.all_time += interval_time ;
    return temp ;
}


void date::del_day(){ all_time -= 1440 ; }

void date::del_hour(){ all_time -= 60 ; }

void date::del_minute(){ all_time -= 1 ; }

date date::operator-(int interval_time)
{
    date temp = *this ;
    temp.all_time -= interval_time ;
    return temp ;
}

bool date::operator<(const date &other) const {
    return all_time < other.all_time ;
}

bool date::operator>(const date &other) const {
    if ( other < *this ) return true ;
    return false ;
}

bool date::operator==(const date &other) const {
    if ( *this < other || other < *this ) return false ;
    return true ;
}

int date::get_date_index() {
    int temp_index = ( all_time - all_month_add_up[6] ) / ( 24 * 60 ) + 1 ;
    return temp_index ;
}

bool date::isSameDay(date other_date) {
    return ( ( all_time / ( 24 * 60 ) ) == ( other_date.all_time / ( 24 * 60 ) ) ) ;
}

void date::print_date()
{
    if ( all_time == 0 ){
        cout << "xx-xx xx:xx" ;
        return ;
    }
    int temp_time = all_time , temp_month = get_month() , temp_day = get_day() , temp_hour = get_hour() , temp_minute = get_minute() ;
    cout << 0 << temp_month << '-' ;
    if ( temp_day < 10 ) cout << 0 ;
    cout << temp_day << ' ' ;
    if ( temp_hour < 10 ) cout << 0 ;
    cout << temp_hour << ':' ;
    if ( temp_minute < 10 ) cout << 0 ;
    cout << temp_minute ;
}

void date::get_other_day(date other_date) {
 //   month = other_date.month ; day = other_date.day ;
    int other_date_time = (other_date.all_time / ( 24 * 60 )) * ( 24 * 60 ) ;
    all_time = all_time % ( 24 * 60 ) + other_date_time ;
}

void date::get_other_time(date other_date) {
    //   hour = other_date.hour ; minute = other_date.minute ;
    int other_date_time = ( other_date.all_time % ( 24 * 60 ) ) ;
    all_time = ( all_time / ( 24 * 60 ) ) * ( 24 * 60 ) + other_date_time ;
}

int date::operator-(date other_date) const
{
    return all_time - other_date.all_time ;
}

ostream &operator<<( ostream &os , const date &temp_date )
{
//    os << '0' << temp_date.month << '-' << temp_date.day << ' ' << temp_date.hour << ':' << temp_date.minute ;
    os << temp_date.all_time ;
    return os ;
}

void date::become_last_minute()
{
    all_time = (all_time / ( 24 * 60 )) * ( 24 * 60 ) ;
    all_time += 1439 ;
}

void date::become_first_minute()
{
    all_time = (all_time / ( 24 * 60 )) * (24 * 60 ) ;
}

int date::get_month()
{
    int temp_time = all_time ;
    for ( int i = 1 ; i < 12 ; i++ ){
        if ( temp_time >= all_month_add_up[i] && temp_time < all_month_add_up[i+1] ) return i ;
    }
    return 12 ;
}

int date::get_day()
{
    int temp_time = all_time , temp_month = get_month() ;
    temp_time -= all_month_add_up[temp_month] ;
    return temp_time / ( 60 * 24 ) + 1 ;
}

int date::get_hour()
{
    return ( all_time / 60 ) % 24 ;
}

int date::get_minute()
{
    return ( all_time % 60 ) ;
}

date date::temp_first_minute()
{
    date temp_date = *this ;
    temp_date.become_first_minute() ;
    return temp_date ;
}

date date::temp_last_minute()
{
    date temp_date = *this ;
    temp_date.become_last_minute() ;
    return temp_date;
}


para::para(stringstream &input_stream)
{
    string temp_key , argument ;
    while ( !input_stream.eof() ){
        input_stream >> temp_key ;
        if ( temp_key == "-c" ){ input_stream >> c ; }
        else if ( temp_key == "-u" ) { input_stream >> u ; }
        else if ( temp_key == "-p" ) { input_stream >> p ; }
        else if ( temp_key == "-n" ) { input_stream >> n ; }
        else if ( temp_key == "-m" ) { input_stream >> m ; }
        else if ( temp_key == "-g" ) { input_stream >> g ; }
        else if ( temp_key == "-i" ) { input_stream >> i ; }
        else if ( temp_key == "-s" ) { input_stream >> s ; }
        else if ( temp_key == "-x" ) { input_stream >> x ; }
        else if ( temp_key == "-t" ) { input_stream >> t ; }
        else if ( temp_key == "-o" ) { input_stream >> o ; }
        else if ( temp_key == "-d" ) { input_stream >> d ; }
        else if ( temp_key == "-y" ) { input_stream >> y ; }
        else if ( temp_key == "-q" ) { input_stream >> q ; }
        else if ( temp_key == "-f" ) { input_stream >> f ; }
    }
}

user::user(para &input_para)
{
//    stringstream change_stream ;
//    change_stream << input_para.u << " " << input_para.p << " " << input_para.n << " " << input_para.m << " " << input_para.g ;
//    change_stream >> user_name >> password >> chinese_name >> mailAddr >> privilege ;
    strcpy(user_name,input_para.u.c_str()) ;
    strcpy(password,input_para.p.c_str()) ;
    strcpy(chinese_name,input_para.n.c_str()) ;
    strcpy(mailAddr,input_para.m.c_str());
    privilege = str_to_int(input_para.g) ;
}

bool user::operator<(const user &other_user) const {
    return ( strcmp( user_name , other_user.user_name ) < 0 ) ;
}

user& user::operator=(const user &other_user) {
    if ( this == &other_user ) return *this ;
    strcpy( user_name,other_user.user_name ) ;
    strcpy( password , other_user.password ) ;
    strcpy( chinese_name , other_user.chinese_name ) ;
    strcpy( mailAddr , other_user.mailAddr ) ;
    privilege = other_user.privilege ;
//    deal_sum = other_user.deal_sum ;
    return *this ;
}

bool user::right_password(string &input_password) {
    string my_password(password) ;
    return my_password == input_password ;
}

void user::print_user()
{
    cout << user_name << " " << chinese_name << " " << mailAddr << " " << privilege << endl ;
}

//void user::change_deal() { deal_sum++ ; }

bool user::operator==(const user &other_user) const
{
    return ( strcmp(user_name,other_user.user_name) == 0 ) ;
}

bool user::operator>( const user &other_user ) const
{
    return ( strcmp(user_name,other_user.user_name) > 0 );
}

ostream &operator<<(ostream &os, const user &temp_user){
//    os << "user: " << temp_user.user_name << " deal_sum -> " << temp_user.deal_sum  ;
    return os ;
}


train::train(para &input_para)
{
    stringstream change_stream , ans_stream ; string temp_str , time_str , date_str ;
    int travel_time[MAX_STATION_SUM] = {0} , stop_over_time[MAX_STATION_SUM] = {0} ;
    change_stream << input_para.i << " " << input_para.n << " " << input_para.m << " " << input_para.s << " " << input_para.p << " " << input_para.x << " " << input_para.t << " " << input_para.o << " " << input_para.d << " " << input_para.y ;
    change_stream >> trainID >> station_num >> seat_num ;
    change_stream >> temp_str ; // -s stations
    get_split_context(temp_str,ans_stream) ;
    for ( int i = 1 ; i <= station_num ; i++ ){
        ans_stream >> all_station[i] ;
    }
    ans_stream.str("") ;
    ans_stream.clear() ;
    change_stream >> temp_str ; // -p prices
    get_split_context(temp_str,ans_stream) ;
    for ( int i = 2 ; i <= station_num ; i++ ){
        ans_stream >> all_price[i] ;
        all_price[i] += all_price[i-1] ; // 前缀和
    }
    ans_stream.str("") ;
    ans_stream.clear() ;
    change_stream >> time_str >> temp_str ; // -x -t travelTimes
    get_split_context(temp_str,ans_stream) ;
    for ( int i = 2 ; i <= station_num ; i++ ){
        ans_stream >> travel_time[i] ;
    }
    ans_stream.str("") ;
    ans_stream.clear() ;
    change_stream >> temp_str ; // -o stop_over_time
    get_split_context(temp_str,ans_stream) ;
    for ( int i = 2 ; i <= station_num - 1 ; i++ ){
        ans_stream >> stop_over_time[i] ;
    }
    ans_stream.str("") ;
    ans_stream.clear() ;
    change_stream >> temp_str >> train_type ; // -d sale_range
    get_split_context(temp_str,ans_stream) ;
    ans_stream >> date_str ;  sale_begin = date(date_str,time_str) ;
    ans_stream >> date_str ; sale_end = date(date_str,time_str) ;

    // todo 处理前缀和

    all_set_off[1] = sale_begin ;
    int pre_sum = 0 ;
    for ( int i = 2 ; i <= station_num - 1 ; i++ ){
        pre_sum += travel_time[i] ;
        all_arrive_in[i] = all_set_off[i-1] + travel_time[i] ;
        pre_sum += stop_over_time[i] ;
        all_set_off[i] = all_arrive_in[i] + stop_over_time[i] ;
    }
    all_arrive_in[station_num] = all_set_off[station_num-1] + travel_time[station_num] ;
}

bool train::operator<(const train &other_train) const
{
    return ( strcmp( trainID , other_train.trainID ) < 0 ) ;
}

void train::release_train(){ isReleased = true ; }

//void train::change_waiting_length(int i){ waiting_length++ ; }

int train::get_location(string &input_location)
{
     char search_train[MAX_STR_LENGTH] = {0} ;
    strcpy(search_train,input_location.c_str()) ;
    for ( int i = 1 ; i <= station_num ; i++ ){
        if ( strcmp( all_station[i] , search_train ) == 0 ) return i ;
    }
    return -1 ;
}

bool train::is_released() { return isReleased ; }

bool train::in_sale( date purchase_day , int location )
{
    date temp_date_1 = purchase_day , temp_date_2 = purchase_day ;
    temp_date_1.become_last_minute() ; // 一天的最后一刻搭车
    temp_date_2.become_first_minute();  // 一天的第一刻搭车
    temp_date_1 = temp_date_1 - ( all_set_off[location] - all_set_off[1] ) ; // all_departure[i] = all_set_off[i] - all_set_off[1]
    temp_date_2 = temp_date_2 - ( all_set_off[location] - all_set_off[1] ) ;
    if ( temp_date_1 < sale_begin  ) return false ;
    if ( temp_date_2 > sale_end  ) return false ;
    return true ;
}

void train::print_travel( date purchase_day, int location_1, int location_2) // todo print_travel 与 all_seat 已经分离
{
    date set_off_date , arrive_in_date ;
    set_off_date.get_other_day(purchase_day) ;
    set_off_date.get_other_time(all_set_off[location_1]) ;
    arrive_in_date = set_off_date + ( all_arrive_in[location_2] - all_set_off[location_1] )  ;
    cout << trainID << " " << all_station[location_1] << " " ;
    set_off_date.print_date() ;
    cout << " -> " << all_station[location_2] << " " ;
    arrive_in_date.print_date() ;
    cout << " " << all_price[location_2] - all_price[location_1] ;
}

void train::combined_print_train( date query_day , day_train &temp_day_train )
{
    date err_date  ;
    query_day.get_other_time(sale_begin) ;
    int index_day = query_day.get_date_index() ;
    cout << trainID << " " << train_type << endl ;
    cout << all_station[1] << " " ; err_date.print_date() ;
    for ( int i = 2 ; i <= station_num ; i++ ){
        cout << " -> " ;
        query_day.print_date() ;
        cout << " " << all_price[i-1] << " " << temp_day_train.seat_num[i-1] << endl ;
        cout << all_station[i] << " " ;
        query_day = query_day + ( all_arrive_in[i] - all_set_off[i-1] ) ;
        query_day.print_date() ;
        query_day = query_day + ( all_set_off[i] - all_arrive_in[i] ) ;
    }
    cout << " -> " ; err_date.print_date() ;
    cout << " " << all_price[station_num] << " x" << endl ;
}

void train::print_train(date query_day) // todo only print unreleased train
{
    date err_date  ;
    query_day.get_other_time(sale_begin) ;
    int index_day = query_day.get_date_index() ;
    cout << trainID << " " << train_type << endl ;
    cout << all_station[1] << " " ; err_date.print_date() ;
    for ( int i = 2 ; i <= station_num ; i++ ){
        cout << " -> " ;
        query_day.print_date() ;
        cout << " " << all_price[i-1] << " " << seat_num << endl ;
        cout << all_station[i] << " " ;
        query_day = query_day + ( all_arrive_in[i] - all_set_off[i-1] ) ;
        query_day.print_date() ;
        query_day = query_day + ( all_set_off[i] - all_arrive_in[i] ) ;
    }
    cout << " -> " ; err_date.print_date() ;
    cout << " " << all_price[station_num] << " x" << endl ;
}

int train::get_price(int location_1, int location_2)
{
    return all_price[location_2] - all_price[location_1] ;
}

int train::get_time(int location_1, int location_2)
{
    return all_arrive_in[location_2] - all_set_off[location_1] ;
}

train &train::operator=(const train &other)
{
    if ( this == &other ) return *this ;
    strcpy( trainID , other.trainID ) ;
    for ( int i = 0 ; i < MAX_STATION_SUM ; i++ ){
        strcpy( all_station[i] , other.all_station[i] ) ;
        all_price[i] = other.all_price[i] ;
        all_set_off[i] = other.all_set_off[i] ;
        all_arrive_in[i] = other.all_arrive_in[i] ;
    }
    train_type = other.train_type ;
    sale_begin = other.sale_begin ;
    sale_end = other.sale_end ;
    isReleased = other.isReleased ;
    seat_num = other.seat_num ;
    station_num = other.station_num ;
    return *this ;
}

bool train::operator==(const train &other_train) const{ return ( strcmp( trainID , other_train.trainID ) == 0 ) ;}

bool train::operator>(const train &other_train) const{ return ( strcmp( trainID , other_train.trainID ) > 0 ) ; }

ostream &operator<<(ostream &os, const train &temp_train) {
    os << "train: " << temp_train.trainID ;
    return os ;
}

bool train::can_take_in_time( date &arrive_in_date , int location )
{
    date temp_date = arrive_in_date ;
    temp_date.get_other_time(all_set_off[location]) ;
    if ( temp_date < arrive_in_date ) temp_date.add_day() ;
    arrive_in_date = temp_date ;
    if ( in_sale(arrive_in_date,location) ) return true ;
    temp_date = temp_date - (all_set_off[location]-all_set_off[1]) ; // 去到可以发车那一刻
    if ( temp_date < sale_begin ){
        arrive_in_date = all_set_off[location] ;
        return true ;
    }
    return false ;
}

date train::get_date_index( int location , date purchase_day )
{
    date temp_date = purchase_day ;
    temp_date.get_other_time(all_set_off[location]) ;
    temp_date = temp_date - ( all_set_off[location] - all_set_off[1] ) ;
    temp_date.become_first_minute() ;
    return temp_date ;
}

IndexKey::IndexKey(string &input_str)
{
    strcpy(real_key,input_str.c_str()) ;
}

IndexKey::IndexKey( const char *input_str )
{
    strcpy(real_key,input_str) ;
}

bool IndexKey::operator<(const IndexKey &other_key) const { return ( strcmp( real_key , other_key.real_key ) < 0 ) ; }

IndexKey::IndexKey(user &input_user){ strcpy( real_key , input_user.user_name ) ; }

IndexKey::IndexKey(train &input_train){ strcpy( real_key , input_train.trainID ) ; }

bool IndexKey::operator==(const IndexKey &other_key) const { return strcmp(real_key,other_key.real_key) == 0 ;}

bool IndexKey::operator>(const IndexKey &other_key) const { return strcmp(real_key,other_key.real_key) > 0 ; }

bool IndexKey::operator>=(const IndexKey &other_key) const { return (*this) > other_key || (*this) == other_key ; }

bool IndexKey::operator<=(const IndexKey &other_key) const { return (*this) < other_key || (*this) == other_key ; }

ostream &operator<<(ostream &os , const IndexKey &temp_key) {
    os << "IndexKey " << temp_key.real_key  ;
    return os ;
}

ticket_deal::ticket_deal(para &input_para)
{
    strcpy(user_name,input_para.u.c_str()) ;
    strcpy(trainID,input_para.i.c_str()) ;
    strcpy(from_location,input_para.f.c_str()) ;
    strcpy(to_location,input_para.t.c_str()) ;
    ticket_num = str_to_int(input_para.n) ;
    departure_time = date(input_para.d) ; // todo 把这个留给 modify
}

bool ticket_deal::operator<(const ticket_deal &other_deal) const
{
    return deal_priority < other_deal.deal_priority ;
}

void ticket_deal::change_status(ticket_status temp_status) { deal_status = temp_status ; }

void ticket_deal::modify_priority(int train_priority) { deal_priority = train_priority ; }

void ticket_deal::modify_time(date from_time, date to_time) { departure_time = from_time ; arrival_time = to_time ; }

void ticket_deal::modify_price(int deal_price) { price = deal_price ; }

void ticket_deal::print_deal()
{
    switch (deal_status) {
        case succeed: cout << "[success] " ; break ;
        case pending: cout << "[pending] " ; break ;
        case refunded: cout << "[refunded] " ; break ;
    }
    cout << trainID << " " << from_location << " " ;
    departure_time.print_date() ;
    cout << " -> " << to_location << " " ;
    arrival_time.print_date() ;
    cout << " " << price << " " << ticket_num << endl ;
}

bool ticket_deal::operator==(const ticket_deal &other_deal) const {
    return !( (*this) < other_deal ) && !( other_deal < (*this) ) ;
}

bool ticket_deal::operator>(const ticket_deal &other_deal) const {
    return other_deal < (*this) ;
}

ticket_deal &ticket_deal::operator=(const ticket_deal &other)
{
    if ( this == &other ) return *this ;
    strcpy( user_name , other.user_name ) ;
    strcpy( trainID , other.trainID ) ;
    strcpy( from_location , other.from_location ) ;
    strcpy( to_location , other.to_location ) ;
    price = other.price ;
    ticket_num = other.ticket_num ;
    deal_priority = other.deal_priority ;
    departure_time = other.departure_time ;
    arrival_time = other.arrival_time ;
    deal_status = other.deal_status ;
    train_set_off = other.train_set_off ;
    int_location_1 = other.int_location_1 ;
    int_location_2 = other.int_location_2 ;
    return *this ;
}

ostream &operator<<( ostream &os , const ticket_deal &temp_deal ) {
    os << "ticket_deal: trainID -> " <<  temp_deal.trainID << " user -> " << temp_deal.user_name << " status -> " ;
    switch (temp_deal.deal_status) {
        case succeed : os << "[success]" ; break ;
        case pending : os << "[pending]" ; break ;
        case refunded : os << "[refunded]" ; break ;
    }
    os << " ticket_num -> " << temp_deal.ticket_num  << " departure_time -> " << temp_deal.departure_time << " arrival_time -> " << temp_deal.arrival_time ;
    return os ;
}

void ticket_deal::ticket_modify( train &temp_train, int location_1, int location_2, date purchase_day)
{
    price = temp_train.get_price(location_1,location_2) ;
    departure_time = purchase_day ;
    departure_time.get_other_time(temp_train.all_set_off[location_1]) ;
    arrival_time = departure_time + temp_train.get_time(location_1,location_2) ;
    int_location_1 = location_1 ;
    int_location_2 = location_2 ;
    train_set_off =  departure_time - ( temp_train.all_set_off[location_1] - temp_train.all_set_off[1] ) ;
    train_set_off.become_first_minute() ;
}


int day_train::get_max_available_ticket( int location_1 , int location_2 )
{
    int min_num = 10000000 ;
    for ( int i = location_1 ; i < location_2 ; i++ ){
        min_num = min_num < seat_num[i] ? min_num : seat_num[i] ;
    }
    return min_num ;
}

void day_train::modify_seat( int location_1 , int location_2 , int ticket_num )
{
    for ( int i = location_1 ; i < location_2 ; i++ ){
        seat_num[i] += ticket_num ;
    }
}

day_train &day_train::operator=(const day_train &temp_day_train)
{
    if ( this == &temp_day_train ) return *this ;
    for ( int i = 0 ; i < MAX_STATION_SUM ; i++ ){
        seat_num[i] = temp_day_train.seat_num[i] ;
    }
    return *this ;
}

bool day_train::operator<( const day_train &other ) const { return false ; }

bool day_train::operator>( const day_train &other ) const { return false ; }

bool day_train::operator==( const day_train &other ) const { return true ; }

