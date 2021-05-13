#include "Engine.h"
using namespace std ;

my_system train_ticket_system ;

string real_command ;

#ifdef my_debug

extern    ofstream core_file(CORE_FILE,ios::out) ;

extern    bool stop_core = false ;

#endif

int main()
{
//    freopen("10.in","r",stdin) ;
//    freopen("temp_out.txt","w",stdout) ;
    while (getline(cin,real_command) ){
        train_ticket_system.process_command(real_command) ;
    }
}
