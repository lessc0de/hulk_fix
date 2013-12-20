
#include "hulk/fix/session.h"
#include "hulk/fix/transport.h"
#include "hulk/fix/transport_callback.h"

using namespace hulk;

fix::fields header;
fix::fields msg;

class test_session : public fix::session
{
public:
    test_session()
    : session()
    {
    }

    test_session( const fix::value& protocol, const fix::fields& header )
    : session( protocol, header )
    {
    }
};

class test_transport : public fix::transport
{
};

struct test_transport_callback : public fix::transport_callback
{
};

void init()
{
    header.push_back( fix::field( 49, "SENDER" ) );
    header.push_back( fix::field( 56, "TARGET" ) );
    msg.push_back( fix::field( 55, "TEST" ) );
}

void test_initiator()
{
    shared_ptr< fix::session > s( new test_session( "FIX.4.4", header ) );
    test_transport t;
    t.attach( s );
    s->send( "D", msg );
    t.close();
}

void test_acceptor()
{
    shared_ptr< fix::session > s1( new test_session );
    shared_ptr< fix::session > s2( new test_session );

    test_transport t1, t2;
    test_transport_callback cb;
    cb.on_open( t1 );
    cb.on_recv( t1, msg );
    t1.attach( s1 );

    cb.on_open( t2 );
    cb.on_recv( t2, msg );
    t2.attach( s2 );

    cb.on_close( t2 );
    cb.on_close( t1 );
}

void test_drop_copy()
{
    test_transport t1, t2;
    test_transport_callback cb;

    cb.on_open( t1 );
    shared_ptr< fix::session > s1( new test_session );
    s1->set_protocol( "FIX.4.4" );
    s1->set_header( header );
    t1.attach( s1 );

    cb.on_open( t2 );
    shared_ptr< fix::session > s2( new test_session );
    s2->set_protocol( "FIX.4.4" );
    s2->set_header( header );
    t2.attach( s2 );

    s1->send( "D", msg );
    s2->send( "D", msg );

    cb.on_close( t2 );
    cb.on_close( t1 );
}

int main( int argc, char** argv )
{
    init();

    test_initiator();
    test_acceptor();
    test_drop_copy();
}
