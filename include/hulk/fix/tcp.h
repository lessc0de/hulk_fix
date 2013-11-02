
#ifndef _hulk_fix_tcp_h_
#define _hulk_fix_tcp_h_

#include "hulk/core/tcp.h"
#include "hulk/core/shared_ptr.h"
#include "hulk/fix/transport_callback.h"

namespace hulk {
namespace  fix {

class session;
class transport_callback;

// -----------------------------------------------------------------------------
class tcp_event_loop
{
public:
    void new_initiator( const char* host, int port, shared_ptr< session >& );
    void new_initiator( const char* host, int port, transport_callback& );
    void new_acceptor( int port, transport_callback& );

    int loop( int timeout=0 );

private:
    ::hulk::tcp_event_loop _eloop;
};

}
}

#endif
