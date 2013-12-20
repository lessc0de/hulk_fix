
#include "hulk/fix/transport.h"
#include "hulk/fix/session.h"
#include "hulk/fix/tcp.h"
#include <cassert>

using namespace hulk;

const int PORT = 5999;

int num_connected = 0;
int num_msgs = 0;
fix::fields msg_recvd;

struct acceptor_callback : public fix::transport_callback
{
    virtual void on_open( fix::transport& t ) {
        ++num_connected;
    }

    virtual void on_recv( fix::transport& t, const fix::fields& flds ) {
        ++num_msgs; msg_recvd = flds;
    }
};

int main( int argc, char** argv )
{
    fix::fields header;
    header.push_back( fix::field( 49, "TEST" ) );
    header.push_back( fix::field( 56, "TEST" ) );

    shared_ptr< fix::session > initiator( new fix::session( "FIX.4.4", header ) );
    shared_ptr< fix::transport_callback > acceptor( new acceptor_callback );

    fix::tcp_event_loop eloop;
    eloop.new_acceptor( PORT, acceptor );
    eloop.new_initiator( "localhost", PORT, initiator );

    fix::fields msg;
    msg.push_back( fix::field( 55, "VOD.L" ) );
    initiator->send( "D", msg );

    eloop.loop();
    eloop.loop();

    assert( 1 == num_connected );
    assert( 1 == num_msgs );
    assert( 9 == msg_recvd.size() );

    fix::field_map fmap( msg_recvd );
    assert( *fmap[55] == "VOD.L" );
}
