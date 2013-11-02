
#include "hulk/fix/session.h"
#include "hulk/fix/transport.h"
#include "hulk/core/logger.h"
#include "hulk/core/shared_ptr.h"

using namespace hulk;

log& l = logger::instance().get( "hulk.fix" );

fix::fields header;
fix::fields msg;

class test_session : public fix::session
{
public:
    test_session() :
    session()
    {
        LOG_INFO( l, "new test_session @ " << this );
    }

    test_session( const fix::value& protocol, const fix::fields& header ) :
    session( protocol, header )
    {
        LOG_INFO( l, "new test_session @ " << this );
    }

    ~test_session()
    {
        LOG_INFO( l, "del test_session @ " << this );
    }

    virtual void recv( const fix::fields& msg, const std::string buf )
    {
        LOG_INFO( l, "test: recv " << msg.size() << " fields" );
    }
};

class test_transport : public fix::transport
{
public:
    test_transport()
    {
        LOG_INFO( l, "new test_transport @ " << this );
    }

    ~test_transport()
    {
        LOG_INFO( l, "del test_transport @ " << this );
    }

    virtual void send( const char* msg, size_t len )
    {
        LOG_INFO( l, "test: send " << len << " bytes" );
    }

    virtual void close()
    {
        LOG_INFO( l, "test: close" );
    }

    virtual void closed()
    {
        LOG_INFO( l, "test: closed" );
    }
};

struct test_transport_callback : public fix::transport_callback {};

void init()
{
    header.push_back( fix::field( 49, "SENDER" ) );
    header.push_back( fix::field( 56, "TARGET" ) );
    msg.push_back( fix::field( 55, "TEST" ) );
}

void test_initiator()
{
    LOG_INFO( l, "--- test_initiator" );

    shared_ptr< fix::session > s( new test_session( "FIX.4.4", header ) );
    test_transport t;
    t.attach( s );
    return;

    s->send( "D", msg );
    t.close();
}

void test_acceptor()
{
    LOG_INFO( l, "--- test_acceptor" );

    shared_ptr< fix::session > s1( new test_session );
    return;

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
    LOG_INFO( l, "--- test_drop_copy" );
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

void x( shared_ptr< fix::session >& a, shared_ptr< fix::session >& b )
{
    a = b;
}

int main( int argc, char** argv )
{
    init();
    test_initiator();
    test_acceptor();
    test_drop_copy();
}
