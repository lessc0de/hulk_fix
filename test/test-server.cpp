
#include "hulk/core/tcp.h"
#include "hulk/core/stopwatch.h"
#include "hulk/core/logger.h"
#include "hulk/fix/decoder.h"
#include "hulk/fix/session.h"

#include <iostream>

using namespace hulk;

fix::decoder* decoder;
fix::session* session;
core::stopwatch watch;

struct my_publisher : public fix::session::publisher
{
    virtual void publish( const std::string& msg ) {
        ;
    }
};

struct my_processor : public fix::session::processor
{
    virtual void process( const fix::fields& msg )
    {
        static int count = 1;
        if( count++ % 100000 == 0 )
        {
            float s = watch.elapsed_s();
            std::cout << "recvd: " << count << " in " << s << " secs, rate " << (float)count/s << std::endl;
        }
    }
};

struct server_callback : public core::tcp_loop::callback
{
    virtual void on_connect( int fd ) {
        LOG_INFO( "server: " << fd << " connected" );
        watch.start();
    }

    virtual void on_disconnect( int fd ) {
        LOG_INFO( "server: " << fd << " disconnected" );
    }

    virtual void on_recv( int fd, const char* data, size_t len ) {
        decoder->decode( data, len );
    }
};

int main( int argc, char** argv )
{
    const char* host = "localhost";
    const int port = 8000;

    my_publisher mpub;
    my_processor mprc;

    fix::fields flds;
    flds[49] = "SENDER";
    flds[56] = "TARGET";
    session = new fix::session( "FIX.4.4", flds, mpub, mprc );
    decoder = new fix::decoder( *session );

    server_callback server_cb;
    core::tcp_loop loop;
    loop.tcp_server( port, server_cb );
    loop.start();
}
