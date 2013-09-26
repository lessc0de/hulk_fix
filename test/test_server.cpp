
#include "hulk/fix/tcp.h"

using namespace hulk::fix;

int main( int argc, char** argv )
{
    tcp_event_loop eloop;
    eloop.new_acceptor< session >( 8001 );
    while( 1 ) {
        eloop.loop();
    }
}
