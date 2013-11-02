
#include "hulk/fix/transport.h"
#include "hulk/fix/session.h"

using namespace hulk::fix;

transport::~transport()
{
    detach();
    close();
}

void transport::send( const fields& msg )
{
    // send( msg -> char array )
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
        _session->recv( msg, str );
    }
}

void transport::attach( shared_ptr< session >& s )
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

void transport::operator()( const fields& msg, const std::string& buf )
{
    recv( msg );
}
