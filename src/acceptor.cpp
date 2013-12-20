
#include "hulk/fix/acceptor.h"
#include "hulk/fix/transport.h"
#include "hulk/fix/session_factory.h"
#include "hulk/core/shared_ptr.h"

using namespace hulk;
using namespace hulk::fix;

acceptor::acceptor( const shared_ptr< session_factory >& f )
: _factory( f )
{
}

void acceptor::on_open( transport& t )
{
}

void acceptor::on_close( transport& t )
{
}

void acceptor::on_recv( transport& t, const fields& f )
{
    if( !t.attached() )
    {
        shared_ptr< session > s( _factory->create() );
        t.attach( s );
    }

    t.recv( f );
}
