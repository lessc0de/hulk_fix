
#ifndef _hulk_fix_tcp_initiator_h_
#define _hulk_fix_tcp_initiator_h_

#include "hulk/core/tcp.h"
#include "hulk/fix/transport.h"

namespace hulk {
namespace  fix {

// -----------------------------------------------------------------------------
class tcp_transport : public transport
{
public:
    inline tcp_transport( int fd );
    inline int get_fd();

    inline virtual void send( const char* msg, size_t len );
    virtual ~tcp_transport() {}

private:
    int _fd;
};

// -----------------------------------------------------------------------------
struct tcp_callback : public ::hulk::core::tcp::callback
{
    tcp_callback( tcp_transport& t ) : _transport( t ) {}

    virtual void on_open( int fd ) {}
    virtual void on_close( int fd ) {}
    virtual void on_recv( int fd, const char* data, size_t len );

    tcp_transport& _transport;
};

// -----------------------------------------------------------------------------
class tcp_event_loop
{
public:
    template< class TSession >
    TSession* new_initiator( const char* host, int port, const value& protocol, const fields& header );

    int loop( int timeout=0 );

private:
    ::hulk::core::tcp::event_loop _eloop;
};

// -----------------------------------------------------------------------------
tcp_transport::tcp_transport( int fd ) : _fd( fd )
{
}

int tcp_transport::get_fd()
{
    return _fd;
}

void tcp_transport::send( const char* msg, size_t len )
{
    int ret = ::send( _fd, msg, len, 0 );
}

// -----------------------------------------------------------------------------
void tcp_callback::on_recv( int fd, const char* data, size_t len )
{
    _transport.recv( data, len );
}

// -----------------------------------------------------------------------------
template< class TSession >
TSession* tcp_event_loop::new_initiator( const char* host, int port, const value& protocol, const fields& header )
{
    int fd = ::hulk::core::tcp::connect( host, port );
    ::hulk::core::tcp::non_blocking( fd );
    tcp_transport* transport = new tcp_transport( fd );
    _eloop.watch( fd, false, *new tcp_callback( *transport ) );
    return new TSession( protocol, header, *transport );
}

int tcp_event_loop::loop( int timeout )
{
    _eloop.loop( timeout );
}

}
}

#endif
