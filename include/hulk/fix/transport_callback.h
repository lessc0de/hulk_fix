
#ifndef _hulk_fix_transport_callback_h_
#define _hulk_fix_transport_callback_h_

#include "hulk/fix/message.h"

namespace hulk {
namespace  fix {

class transport;

struct transport_callback
{
    virtual void on_open( transport& ) {}
    virtual void on_close( transport& ) {}
    virtual void on_recv( transport&, const fields& ) {}
};

}
}

#endif
