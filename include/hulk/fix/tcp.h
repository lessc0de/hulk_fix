
#ifndef _hulk_fix_tcp_initiator_h_
#define _hulk_fix_tcp_initiator_h_

#include "hulk/core/tcp.h"
#include "hulk/fix/transport.h"

#include <map>

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
struct tcp_initiator_callback : public ::hulk::core::tcp::callback
{
    tcp_initiator_callback( tcp_transport& t ) : _transport( t ) {}

    virtual void on_open( int fd ) {}
    virtual void on_close( int fd ) {}
    virtual void on_recv( int fd, const char* data, size_t len );

    tcp_transport& _transport;
};

// -----------------------------------------------------------------------------
struct tcp_acceptor_callback : public ::hulk::core::tcp::callback
{
    tcp_acceptor_callback() {}

    virtual void on_open( int fd ) {}
    virtual void on_close( int fd );
    virtual void on_recv( int fd, const char* data, size_t len );

    typedef std::map< int, tcp_transport* > transport_map;
    transport_map _transports;;
};

// -----------------------------------------------------------------------------
class tcp_event_loop
{
public:
    template< class TSession >
    TSession* new_initiator( const char* host, int port, const value& protocol, const fields& header );
    void new_acceptor( int port );

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
void tcp_initiator_callback::on_recv( int fd, const char* data, size_t len )
{
    _transport.recv( data, len );
}

// -----------------------------------------------------------------------------
void tcp_acceptor_callback::on_close( int fd )
{
    transport_map::iterator it = _transports.find( fd );
    if( it != _transports.end() )
    {
        delete it->second->get_session();
        delete it->second;
        _transports.erase( it );
    }
}

void tcp_acceptor_callback::on_recv( int fd, const char* data, size_t len )
{
    tcp_transport* transport;
    transport_map::iterator it = _transports.find( fd );
    if( it == _transports.end() )
    {
        transport = new tcp_transport( fd );
        new session( *transport );
        _transports[fd] = transport;
    }
    else
    {
        transport = it->second;
    }

    transport->recv( data, len );
}

// -----------------------------------------------------------------------------
template< class TSession >
TSession* tcp_event_loop::new_initiator( const char* host, int port, const value& protocol, const fields& header )
{
    int fd = ::hulk::core::tcp::connect( host, port );
    ::hulk::core::tcp::non_blocking( fd );
    tcp_transport* transport = new tcp_transport( fd );
    _eloop.watch( fd, false, *new tcp_initiator_callback( *transport ) );
    return new TSession( protocol, header, *transport );
}

void tcp_event_loop::new_acceptor( int port )
{
    int fd = ::hulk::core::tcp::bind( port, 1024 );
    ::hulk::core::tcp::non_blocking( fd );
    _eloop.watch( fd, true, *new tcp_acceptor_callback() );
}

int tcp_event_loop::loop( int timeout )
{
    _eloop.loop( timeout );
}

}
}

#endif
