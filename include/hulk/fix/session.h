
#ifndef _hulk_fix_session_h_
#define _hulk_fix_session_h_

#include "hulk/core/shared_ptr.h"
#include "hulk/fix/message.h"
#include <stdint.h>

namespace hulk {
namespace  fix {

class transport;

class session
{
public:
    friend class transport;

    session();
    session( const value& protocol, const fields& header );
    virtual ~session();

    void set_protocol( const value& protocol );
    void set_header( const fields& header );
    void send( const value& msg_type, const fields& msg, std::string* copy_buf=0 );

    virtual void on_recv( const fields& msg, const std::string buf );

private:
    void set_transport( transport* transport );

    value _protocol;
    fields _header;

    transport* _transport;

    uint64_t _seq_in;
    uint64_t _seq_out;
};

}
}

#endif
