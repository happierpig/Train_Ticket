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
//    freopen("1.in","r",stdin) ;
    while (getline(cin,real_command) ){
        train_ticket_system.process_command(real_command) ;
    }
}
