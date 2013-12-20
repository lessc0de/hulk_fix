
#ifndef _hulk_fix_acceptor_h_
#define _hulk_fix_acceptor_h_

#include "hulk/core/shared_ptr.h"
#include "hulk/fix/transport_callback.h"

namespace hulk {
namespace  fix {

class session_factory;

struct acceptor : public transport_callback
{
public:
    acceptor( const shared_ptr< session_factory >& f );

    virtual void on_open( transport& );
    virtual void on_close( transport& );
    virtual void on_recv( transport&, const fields& );

private:
    shared_ptr< session_factory > _factory;
};

}
}

#endif
