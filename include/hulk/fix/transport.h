
#ifndef _hulk_fix_transport_h_
#define _hulk_fix_transport_h_

#include "hulk/fix/session.h"
#include "hulk/fix/decoder.h"

namespace hulk {
namespace  fix {

// -----------------------------------------------------------------------------
class transport
{
public:
    inline transport();
    inline void set_session( session& s );
    inline session* get_session();
    inline void recv( const fields& msg, const std::string buf );
    inline void recv( const char* msg, size_t len );
    inline void operator()( const fields& msg, const std::string buf );

    virtual void send( const char* msg, size_t len ) {}
    virtual ~transport() {}

private:
    decoder _decoder;
    session* _session;
};

// -----------------------------------------------------------------------------
transport::transport() : _session( 0 )
{
}

void transport::recv( const fields& msg, const std::string buf )
{
    if( _session ) {
        _session->recv( msg, buf );
    }
}

void transport::recv( const char* msg, size_t len )
{
    _decoder.decode( *this, msg, len );
}

void transport::set_session( session& s )
{
    _session = &s;
}

session* transport::get_session()
{
    return _session;
}

void transport::operator()( const fields& msg, const std::string buf )
{
    recv( msg, buf );
}

}
}

#endif
