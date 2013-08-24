
#ifndef _hulk_fix_transport_h_
#define _hulk_fix_transport_h_

#include "hulk/fix/session.h"
#include "hulk/fix/decoder.h"

#include <sys/socket.h>

namespace hulk {
namespace  fix {

// -----------------------------------------------------------------------------
class transport
{
public:
    inline transport();
    inline void set_session( session& s );
    inline void recv( const fields& msg );
    inline void recv( const char* msg, size_t len );
    inline void operator()( const fields& msg );

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

void transport::recv( const fields& msg )
{
    if( _session ) {
        _session->recv( msg );
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

void transport::operator()( const fields& msg )
{
    recv( msg );
}

}
}

#endif
