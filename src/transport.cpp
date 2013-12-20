
#include "hulk/fix/transport.h"
#include "hulk/fix/session.h"

using namespace hulk::fix;

transport::~transport()
{
    detach();
    close();
}

void transport::recv( const char* data, size_t len )
{
    _decoder.decode( *this, data, len );
}

void transport::recv( const fields& msg )
{
    if( _session )
    {
        std::string str;
        _session->on_recv( msg, str );
    }
}

bool transport::attached()
{
    return _session;
}

void transport::attach( const shared_ptr< session >& s )
{
    _session = s;
    _session->set_transport( this );
}

void transport::detach()
{
    if( _session )
    {
        _session->set_transport( 0 );
        _session.reset();
    }
}

void transport::closed()
{
    detach();
}

void transport::operator()( const fields& msg, const std::string& buf )
{
    recv( msg );
}
