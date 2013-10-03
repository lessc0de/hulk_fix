
#ifndef _hulk_fix_tcp_initiator_h_
#define _hulk_fix_tcp_initiator_h_

#include "hulk/core/tcp.h"
#include "hulk/fix/transport.h"

#include <sys/socket.h>

namespace hulk {
namespace  fix {

// -----------------------------------------------------------------------------
class tcp_transport : public transport
{
public:
    inline tcp_transport( int fd );
    inline int get_fd();

    virtual void send( const char* msg, size_t len );
    virtual ~tcp_transport() {}

private:
    int _fd;
};

// -----------------------------------------------------------------------------
struct tcp_initiator_callback : public ::hulk::tcp_callback
{
    tcp_initiator_callback( tcp_transport& t );

    virtual void on_recv( tcp_context& c, const char* data, size_t len );

    tcp_transport& _transport;
};

// -----------------------------------------------------------------------------
template< class TSession >
struct tcp_acceptor_callback : public ::hulk::tcp_callback
{
    tcp_acceptor_callback() {}

    virtual void on_open( tcp_context& c );
    virtual void on_close( tcp_context& c );
    virtual void on_recv( tcp_context& c, const char* data, size_t len );
};

// -----------------------------------------------------------------------------
class tcp_event_loop
{
public:
    template< class TSession >
    TSession* new_initiator( const char* host, int port, const value& protocol, const fields& header );

    template< class TSession >
    void new_acceptor( int port );

    inline int loop( int timeout=0 );

private:
    ::hulk::tcp_event_loop _eloop;
};

// -----------------------------------------------------------------------------
tcp_transport::tcp_transport( int fd )
: _fd( fd )
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
tcp_initiator_callback::tcp_initiator_callback( tcp_transport& t )
: _transport( t )
{
}

void tcp_initiator_callback::on_recv( tcp_context& c, const char* data, size_t len )
{
    _transport.recv( data, len );
}

// -----------------------------------------------------------------------------
template< class TSession >
void tcp_acceptor_callback< TSession >::on_open( tcp_context& c )
{
    tcp_transport* transport = new tcp_transport( c._fd );
    transport->set_session( *new TSession( *transport ) );
    c._data = transport;
}

template< class TSession >
void tcp_acceptor_callback< TSession >::on_close( tcp_context& c )
{
    tcp_transport* transport = (tcp_transport*)c._data;
    transport->get_session()->set_transport( 0 );
    delete transport;
}

template< class TSession >
void tcp_acceptor_callback< TSession >::on_recv( tcp_context& c, const char* data, size_t len )
{
    tcp_transport* transport = (tcp_transport*)c._data;
    transport->recv( data, len );
}

// -----------------------------------------------------------------------------
template< class TSession >
TSession* tcp_event_loop::new_initiator( const char* host, int port, const value& protocol, const fields& header )
{
    int fd = ::hulk::tcp_connect( host, port );
    ::hulk::tcp_non_blocking( fd );
    tcp_transport* transport = new tcp_transport( fd );
    _eloop.watch( fd, false, *new tcp_initiator_callback( *transport ) );
    return new TSession( protocol, header, *transport );
}

template< class TSession >
void tcp_event_loop::new_acceptor( int port )
{
    int fd = ::hulk::tcp_bind( port, 1024 );
    ::hulk::tcp_non_blocking( fd );
    _eloop.watch( fd, true, *new tcp_acceptor_callback< TSession >() );
}

int tcp_event_loop::loop( int timeout )
{
    _eloop.loop( timeout );
}

}
}

#endif
