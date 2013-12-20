
#include "hulk/fix/transport.h"
#include "hulk/fix/session.h"
#include "hulk/fix/tcp.h"
#include "hulk/core/stopwatch.h"
#include <iostream>

using namespace hulk;

const int NUM_MSGS = 100000;
const int PORT = 5999;

int num_recvd = 0;

struct acceptor_callback : public fix::transport_callback
{
    virtual void on_recv( fix::transport& t, const fix::fields& flds ) {
        ++num_recvd;
    }
};

int main( int argc, char** argv )
{
    fix::fields header;
    header.push_back( fix::field( 49, "TEST" ) );
    header.push_back( fix::field( 56, "TEST" ) );

    fix::fields body;
    body.push_back( fix::field( 55, "VOD.L" ) );

    shared_ptr< fix::session > initiator( new fix::session( "FIX.4.4", header ) );
    shared_ptr< fix::transport_callback > acceptor( new acceptor_callback );

    fix::tcp_event_loop eloop;
    eloop.new_acceptor( PORT, acceptor );
    eloop.new_initiator( "localhost", PORT, initiator );

    stopwatch watch;
    watch.start();

    for( int i=0; i<NUM_MSGS; ++i )
    {
        initiator->send( "D", body );
        eloop.loop();
    }

    float secs = watch.elapsed_s();
    std::cout << num_recvd << " messages in " << secs << " seconds. rate " << num_recvd / secs << " per second\n";
}
