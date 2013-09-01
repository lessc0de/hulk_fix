
#ifndef _hulk_fix_session_h_
#define _hulk_fix_session_h_

#include "hulk/fix/message.h"

#include <stdint.h>

namespace hulk {
namespace  fix {

class transport;

class session
{
public:
    session( transport& );
    session( const value& protocol, const fields& header, transport& );
    virtual ~session();

    void set_protocol( const value& protocol );
    void set_header( const fields& header );
    void send( const value& msg_type, const fields& msg );

    inline transport& get_transport();

    virtual void recv( const fields& msg );

private:
    value _protocol;
    fields _header;

    transport& _transport;

    uint64_t _seq_in;
    uint64_t _seq_out;

};

transport& session::get_transport()
{
    return _transport;
}

}
}

#endif
