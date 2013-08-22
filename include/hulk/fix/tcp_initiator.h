
#ifndef _hulk_fix_tcp_initiator_h_
#define _hulk_fix_tcp_initiator_h_

#include "hulk/core/tcp.h"
#include "hulk/fix/transport.h"

namespace hulk {
namespace  fix {

template< class TSession >
TSession* tcp_initiator( const char* host, int port, const value& protocol, const fields& header )
{
    int fd = hulk::core::tcp::connect( host, port );
    hulk::core::tcp::non_blocking( fd );
    tcp_transport* transport = new tcp_transport( fd );
    return new TSession( protocol, header, *transport );
}

}
}

#endif
