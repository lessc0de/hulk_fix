#include "hulk/core/tcp.h"
#include "hulk/core/logger.h"

#include "hulk/fix/session.h"

#include <sys/socket.h>

using namespace hulk;
using namespace hulk::core;

log& l = logger::instance().get( "hulk.fix" );

struct client_send_handler : public fix::session::send_handler
{
    client_send_handler( int fd ) : _fd( fd ) {
        ;
    }

    virtual void send( const std::string& msg )
    {
        LOG_DEBUG( l, "sending: " << msg );
        ::send( _fd, msg.c_str(), msg.size(), 0 );
    }

    int _fd;
};

struct client_recv_handler : public fix::session::recv_handler
{
    virtual void recv( const fix::fields& msg ) {
        ;
    }
};

struct client_callback : public tcp::callback
{
    virtual void on_open( int fd ) {
        ;
    }

    virtual void on_close( int fd ) {
        LOG_INFO( l, "on_close: " << fd );
    }

    virtual void on_recv( int fd, const char* data, size_t len ) {
        LOG_INFO( l, "on_recv: " << fd << ", " << data );
    }
};

int main( int argc, char** argv )
{
    const char* host = argv[1];
    const int port = atoi( argv[2] );

    LOG_INFO( l, "connecting to " << host << ":" << port );
    int fd = tcp::connect( host, port );
    LOG_INFO( l, "connected!" );

    client_send_handler csh( fd );
    client_recv_handler crh;
    fix::fields header;
    header.push_back( fix::field( 49, "BANZAI" ) );
    header.push_back( fix::field( 56, "EXEC" ) );
    fix::session session( "FIX.4.4", header, csh, crh );

    LOG_INFO( l, "sending logon" );
    fix::fields body;
    header.push_back( fix::field(  98, "0" ) );
    header.push_back( fix::field( 108, "30" ) );
    session.send( "A", body );

    LOG_INFO( l, "starting event loop" );
    client_callback cb;
    tcp::event_loop el( 1024 );
    el.watch( fd, false, cb );
    while( 1 ) {
        el.loop();
    }
}
