
#include "hulk/fix/transport.h"
#include "hulk/fix/encoder.h"
#include "hulk/fix/session.h"
#include "hulk/fix/tcp.h"
#include <cassert>
#include <list>

using namespace hulk;

const int PORT = 5999;

int num_msgs = 0;

struct acceptor_callback : public fix::transport_callback
{
    acceptor_callback()
    {
        fix::fields header;
        fix::fields body;
        encode( _str, 1, "FIX.4.4", "D", header, body );
    }

    void publish()
    {
        std::list< fix::transport* >::const_iterator it;
        for( it = _transports.begin(); it != _transports.end(); ++it )
        {
            (*it)->send( _str.c_str(), _str.size() );
        }
    }

    virtual void on_open( fix::transport& t )
    {
        _transports.push_back( &t );
    }

    std::string _str;
    std::list< fix::transport* > _transports;
};

class initiator : public fix::session
{
public:
    initiator( const fix::value& protocol, const fix::fields& header )
    : fix::session( protocol, header )
    {
    }

    virtual void on_recv( const fix::fields& msg, const std::string buf )
    {
        ++num_msgs;
    }
};

int main( int argc, char** argv )
{
    fix::fields header;
    header.push_back( fix::field( 49, "SENDER" ) );
    header.push_back( fix::field( 56, "TARGET" ) );

    shared_ptr< fix::session > initiator1( new initiator( "FIX.4.4", header ) );
    shared_ptr< fix::session > initiator2( new initiator( "FIX.4.4", header ) );
    shared_ptr< fix::session > initiator3( new initiator( "FIX.4.4", header ) );
    shared_ptr< fix::transport_callback > acceptor( new acceptor_callback );

    fix::tcp_event_loop eloop;
    eloop.new_acceptor( PORT, acceptor );
    eloop.new_initiator( "localhost", PORT, initiator1 );
    eloop.new_initiator( "localhost", PORT, initiator2 );
    eloop.new_initiator( "localhost", PORT, initiator3 );
    eloop.loop();

    ((acceptor_callback*)acceptor.get())->publish();

    eloop.loop();

    assert( 3 == num_msgs );
}
