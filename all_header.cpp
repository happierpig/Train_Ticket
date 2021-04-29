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
    month = date_str[1] - '0' ;
    day = (date_str[3] - '0') * 10 + date_str[4] - '0' ;
}

date::date(string &date_str, string &time_str) : date(date_str)
{
    hour = ( time_str[0] - '0' ) * 10 + time_str[1] - '0' ;
    minute = ( time_str[3] - '0' ) * 10 + time_str[4] - '0' ;
}

void date::add_day()
{
    if ( ( month == 6 && day == 30 ) || ( month == 7 && day == 31 ) ){ month++ ; day = 1 ; return ; }
    day++ ;
}

void date::add_hour()
{
    if ( hour == 23 ) { add_day() ; hour = 0 ; return ; }
    hour++ ;
}

void date::add_minute()
{
    if ( minute == 59 ) { add_hour() ; minute = 0 ; return ; }
    minute++ ;
}

date date::operator+( int interval_time ) {
    date temp = *this ;
    while ( interval_time >= 1440 ){
        temp.add_day() ;
        interval_time -= 1440 ;
    }
    while ( interval_time >= 60 ){
        temp.add_hour() ;
        interval_time -= 60 ;
    }
    while ( interval_time > 0 ){
        temp.add_minute() ;
        interval_time -= 1 ;
    }
    return temp ;
}


void date::del_day()
{
    if ( day == 1 ){
        if ( month == 7 ){ month = 6 ; day = 30 ; return ; }
        if ( month == 8 ){ month = 7 ; day = 31 ; return ; }
    }
    day-- ;
}

void date::del_hour()
{
    if ( hour == 0 ){ del_day() ; hour = 23 ; return ; }
    hour-- ;
}

void date::del_minute()
{
    if ( minute == 0 ) { del_hour() ; minute = 59 ; return ; }
    minute-- ;
}

date date::operator-(int interval_time)
{
    date temp = *this ;
    while ( interval_time >= 1440 ){
        temp.del_day() ;
        interval_time -= 1440 ;
    }
    while ( interval_time >= 60 ){
        temp.del_hour() ;
        interval_time -= 60 ;
    }
    while ( interval_time > 0 ){
        temp.del_minute() ;
        interval_time -= 1 ;
    }
    return temp ;
}

bool date::operator<(const date &other) const {
    if ( month != other.month ) return month < other.month ;
    if ( day != other.day ) return day < other.day ;
    if ( hour != other.hour ) return hour < other.hour ;
    return minute < other.minute ;
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
    int temp_index = 0 ;
    if ( month == 7 ) temp_index += 30 ;
    if ( month == 8 ) temp_index += 61 ;
    temp_index += day ;
    return temp_index ;
}

bool date::isSameDay(date other_date) {
    return ( month == other_date.month && day == other_date.day ) ;
}

void date::print_date()
{
    if ( month == 0 ){
        cout << "xx-xx xx:xx" ;
        return ;
    }
    cout << 0 << month << '-' ;
    if ( day < 10 ) cout << 0 ;
    cout << day << ' ' ;
    if ( hour < 10 ) cout << 0 ;
    cout << hour << ':' ;
    if ( minute < 10 ) cout << 0 ;
    cout << minute ;
}

void date::get_other_day(date other_date) { month = other_date.month ; day = other_date.day ; }

void date::get_other_time(date other_date) { hour = other_date.hour ; minute = other_date.minute ; }

int date::operator-(date other_date)
{
    date temp_date = other_date ;
    int counter = 0 ;
    while( temp_date + 1440 < *this ){
        temp_date.add_day() ;
        counter += 1440 ;
    }
    temp_date.del_day() ;
    counter -= 1440 ;
    while ( temp_date + 60 < *this ){
        temp_date.add_hour() ;
        counter += 60 ;
    }
    temp_date.del_hour() ;
    counter -= 60 ;
    while ( temp_date + 1 < *this ){
        temp_date.add_minute() ;
        counter += 1 ;
    }
    return counter ;
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
    stringstream change_stream ;
    change_stream << input_para.u << " " << input_para.p << " " << input_para.n << " " << input_para.m << " " << input_para.g ;
    change_stream >> user_name >> password >> chinese_name >> mailAddr >> privilege ;
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

void user::change_deal() { deal_sum++ ; }

bool user::operator==(const user &other_user) const
{
    return ( strcmp(user_name,other_user.user_name) == 0 ) ;
}

bool user::operator>( const user &other_user ) const
{
    return ( strcmp(user_name,other_user.user_name) > 0 );
}


train::train(para &input_para)
{
    stringstream change_stream , ans_stream ; string temp_str , time_str , date_str ;
    int travel_time[MAX_STATION_SUM] = {0} , stop_over_time[MAX_STATION_SUM] = {0} ;
    change_stream << input_para.i << " " << input_para.n << " " << input_para.m << " " << input_para.s << " " << input_para.p << " " << input_para.x << " " << input_para.t << " " << input_para.o << " " << input_para.d << " " << input_para.y ;
    change_stream >> trainID >> station_num >> seat_num ;
    for ( int i = 1 ; i <= MAX_DATE - 1 ; i++ ){
        for ( int j = 0 ; j <= station_num ; j++ ){
            all_seat[i][j] = seat_num ;
        }
    }
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
        all_arrival[i] = pre_sum ;
        pre_sum += stop_over_time[i] ;
        all_set_off[i] = all_arrive_in[i] + stop_over_time[i] ;
        all_departure[i] = pre_sum ;
    }
    all_arrive_in[station_num] = all_set_off[station_num-1] + travel_time[station_num] ;
    all_arrival[station_num] = pre_sum + travel_time[station_num] ;
}

bool train::operator<(const train &other_train) const
{
    return ( strcmp( trainID , other_train.trainID ) < 0 ) ;
}

void train::release_train(){ isReleased = true ; }

void train::change_waiting_length(int i){ waiting_length++ ; }

int train::get_location(string &input_location)
{
    stringstream change_stream ; char search_train[MAX_STR_LENGTH] = {0} ;
    change_stream << input_location ; change_stream >> search_train ;
    for ( int i = 1 ; i <= station_num ; i++ ){
        if ( strcmp( all_station[i] , search_train ) == 0 ) return i ;
    }
    return -1 ;
}

bool train::is_released() { return isReleased ; }

bool train::in_sale( date purchase_day , int location )
{
    date temp_date = purchase_day ;
    temp_date = temp_date - all_departure[location] ;
    if ( temp_date < sale_begin && !temp_date.isSameDay(sale_begin) ) return false ;
    if ( temp_date > sale_end && !temp_date.isSameDay(sale_end) ) return false ;
    return true ;
}

void train::ticket_decrease( date purchase_day, int location_1, int location_2, int purchase_ticket )
{
    purchase_day = purchase_day - all_departure[location_1] ;
    int temp_day = purchase_day.get_date_index() ;
    for ( int i = location_1 ; i < location_2 ; i++ ){
        all_seat[temp_day][i] -= purchase_ticket ;
    }
}

bool train::ticket_is_enough(date purchase_day, int location_1, int location_2, int purchase_ticket)
{
    int min_num = get_max_available_ticket(purchase_day,location_1,location_2) ;
    return min_num >= purchase_ticket ;
}

void train::print_travel(date purchase_day, int location_1, int location_2)
{
    date set_off_date , arrive_in_date ;
    set_off_date.get_other_day(purchase_day) ;
    set_off_date.get_other_time(all_set_off[location_1]) ;
    arrive_in_date = set_off_date + ( all_arrival[location_2] - all_departure[location_1] )  ;
    cout << trainID << " " << all_station[location_1] << " " ;
    set_off_date.print_date() ;
    cout << " -> " << all_station[location_2] << " " ;
    arrive_in_date.print_date() ;
    cout << " " << all_price[location_2] - all_price[location_1] ;
}

void train::print_train(date query_day)
{
    date err_date  ;
    query_day.get_other_time(sale_begin) ;
    int index_day = query_day.get_date_index() ;
    cout << trainID << " " << train_type << endl ;
    cout << all_station[1] << " " ; err_date.print_date() ;
    for ( int i = 2 ; i <= station_num ; i++ ){
        cout << " -> " ;
        query_day.print_date() ;
        cout << " " << all_price[i-1] << " " << all_seat[index_day][i-2] << endl ;
        cout << all_station[i] << " " ;
        query_day = query_day + ( all_arrival[i] - all_departure[i-1] ) ;
        query_day.print_date() ;
        query_day = query_day + ( all_departure[i] - all_arrival[i] ) ;
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
    return all_arrival[location_2] - all_departure[location_1] ;
}

int train::get_max_available_ticket(date purchase_day, int location_1, int location_2)
{
    purchase_day = purchase_day - all_departure[location_1] ;
    int temp_day = purchase_day.get_date_index() , min_num = seat_num ;
    for ( int i = location_1 ; i < location_2 ; i++ ){
        min_num = min(min_num,all_seat[temp_day][i]) ;
    }
    return min_num ;
}

train &train::operator=(const train &other)
{
    if ( this == &other ) return *this ;
    strcpy( trainID , other.trainID ) ;
    for ( int i = 0 ; i < MAX_STATION_SUM ; i++ ){
        strcpy( all_station[i] , other.all_station[i] ) ;
        all_price[i] = other.all_price[i] ;
        all_departure[i] = other.all_departure[i] ;
        all_arrival[i] = other.all_arrival[i] ;
        all_set_off[i] = other.all_set_off[i] ;
        all_arrive_in[i] = other.all_arrive_in[i] ;
    }
    train_type = other.train_type ;
    sale_begin = other.sale_begin ;
    sale_end = other.sale_end ;
    isReleased = other.isReleased ;
    seat_num = other.seat_num ;
    station_num = other.station_num ;
    waiting_length = other.waiting_length ;
    for ( int i = 0 ; i < MAX_DATE ; i++ ){
        for ( int j = 0 ; j < MAX_STATION_SUM ; j++ ){
            all_seat[i][j] = other.all_seat[i][j] ;
        }
    }
    return *this ;
}

bool train::operator==(const train &other_train) const{ return ( strcmp( trainID , other_train.trainID ) == 0 ) ;}

bool train::operator>(const train &other_train) const{ return ( strcmp( trainID , other_train.trainID ) > 0 ) ; }

void train::ticket_increase(date purchase_day, int location_1, int location_2, int purchase_ticket)
{
    purchase_day = purchase_day - all_departure[location_1] ;
    int temp_day = purchase_day.get_date_index() ;
    for ( int i = location_1 ; i < location_2 ; i++ ){
        all_seat[temp_day][i] += purchase_ticket ;
    }
}

IndexKey::IndexKey(string &input_str)
{
    stringstream temp_stream ;
    temp_stream << input_str ;
    temp_stream >> real_key ;
}

bool IndexKey::operator<(const IndexKey &other_key) const { return ( strcmp( real_key , other_key.real_key ) < 0 ) ; }

IndexKey::IndexKey(user &input_user){ strcpy( real_key , input_user.user_name ) ; }

IndexKey::IndexKey(train &input_train){ strcpy( real_key , input_train.trainID ) ; }

bool IndexKey::operator==(const IndexKey &other_key) const { return strcmp(real_key,other_key.real_key) == 0 ;}

bool IndexKey::operator>(const IndexKey &other_key) const { return strcmp(real_key,other_key.real_key) > 0 ; }

bool IndexKey::operator>=(const IndexKey &other_key) const { return (*this) > other_key || (*this) == other_key ; }

bool IndexKey::operator<=(const IndexKey &other_key) const { return (*this) < other_key || (*this) == other_key ; }

ticket_deal::ticket_deal(para &input_para)
{
    stringstream change_stream ; string temp_str ; // date_str
    change_stream << input_para.u << " " << input_para.i << " " << input_para.d << " " << input_para.n << " " << input_para.f << " " << input_para.t  ;
    change_stream >> user_name >> trainID >> temp_str >> ticket_num >> from_location >> to_location ;
    departure_time = date(temp_str) ;
}

bool ticket_deal::operator<(const ticket_deal &other_deal) const
{
    if ( isWaiting ) return deal_priority < other_deal.deal_priority ;
    return deal_sequence < other_deal.deal_sequence ;
}

void ticket_deal::change_status(ticket_status temp_status) { deal_status = temp_status ; }

void ticket_deal::modify_waiting(bool need_to_wait) { isWaiting = need_to_wait ; }

void ticket_deal::modify_sequence(int user_sequence) { deal_sequence = user_sequence ; }

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
    deal_sequence = other.deal_sequence ;
    deal_priority = other.deal_priority ;
    isWaiting = other.isWaiting ;
    departure_time = other.departure_time ;
    arrival_time = other.arrival_time ;
    deal_status = other.deal_status ;
    return *this ;
}







