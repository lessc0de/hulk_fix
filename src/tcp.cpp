
#include "hulk/fix/tcp.h"
#include "hulk/fix/session.h"
#include "hulk/fix/transport.h"
#include <sys/socket.h>

using namespace hulk;
using namespace hulk::fix;

// -----------------------------------------------------------------------------
struct tcp_transport : public transport
{
    tcp_transport( int fd, const shared_ptr< transport_callback >& cb )
    : _fd( fd ),
      _callback( cb )
    {
    }

    tcp_transport( int fd, const shared_ptr< session >& s )
    : _fd( fd ),
      _session( s )
    {
    }

    void send( const char* msg, size_t len )
    {
        ::send( _fd, msg, len, 0 );
    }

    void decode( const char* data, size_t len )
    {
        _decoder.decode( *this, data, len );
    }

    void operator()( const fields& msg, const std::string buf )
    {
        if( _callback ) {
            _callback->on_recv( *this, msg );
        } else if( _session ) {
            _session->on_recv( msg, buf );
        }
    }

    int _fd;
    decoder _decoder;
    shared_ptr< transport_callback > _callback;
    shared_ptr< session > _session;
};

// -----------------------------------------------------------------------------
struct tcp_transport_callback : public ::hulk::tcp_callback
{
    tcp_transport_callback( const shared_ptr< transport_callback >& cb )
    : _callback( cb )
    {
    }

    virtual void on_open( tcp_context& ctx )
    {
        ctx._data = new tcp_transport( ctx._fd, _callback );
        _callback->on_open( *(tcp_transport*)ctx._data );
    }

    virtual void on_close( tcp_context& ctx )
    {
        tcp_transport* tpt = (tcp_transport*)ctx._data;
        if( tpt )
        {
            _callback->on_close( *tpt );
            tpt->detach();
            delete tpt;
        }
    }

    virtual void on_recv( tcp_context& ctx, const char* data, size_t len )
    {
        tcp_transport* tpt = (tcp_transport*)ctx._data;
        if( tpt ) {
            tpt->decode( data, len );
        }
    }

    shared_ptr< transport_callback > _callback;
};

// -----------------------------------------------------------------------------
struct session_callback : public ::hulk::tcp_callback
{
    session_callback( const shared_ptr< session >& s )
    : _session( s ),
      _transport( 0 )
    {
    }

    virtual void on_open( tcp_context& ctx )
    {
        _transport = new tcp_transport( ctx._fd, _session );
        _transport->attach( _session );
    }

    virtual void on_close( tcp_context& ctx )
    {
        _transport->detach();
        delete _transport;
        _transport = 0;
    }

    virtual void on_recv( tcp_context& ctx, const char* data, size_t len )
    {
        if( _transport ) {
            _transport->decode( data, len );
        }
    }

    shared_ptr< session > _session;
    tcp_transport* _transport;
};

// -----------------------------------------------------------------------------
void ::hulk::fix::tcp_event_loop::new_initiator( const char* host, int port, const shared_ptr< session >& s )
{
    int fd = ::hulk::tcp_non_blocking( ::hulk::tcp_connect( host, port ) );
    shared_ptr< tcp_callback > cb( new session_callback( s ) );
    _eloop.watch( fd, false, cb );
}

void ::hulk::fix::tcp_event_loop::new_initiator( const char* host, int port, const shared_ptr< transport_callback >& cb )
{
    int fd = ::hulk::tcp_non_blocking( ::hulk::tcp_connect( host, port ) );
    shared_ptr< tcp_callback > tcb( new tcp_transport_callback( cb ) );
    _eloop.watch( fd, false, tcb );
}

void ::hulk::fix::tcp_event_loop::new_acceptor( int port, const shared_ptr< transport_callback >& cb )
{
    int fd = ::hulk::tcp_non_blocking( ::hulk::tcp_bind( port ) );
    shared_ptr< tcp_callback > tcb( new tcp_transport_callback( cb ) );
    _eloop.watch( fd, true, tcb );
}

int ::hulk::fix::tcp_event_loop::loop( int timeout )
{
    _eloop.loop( timeout );
}
