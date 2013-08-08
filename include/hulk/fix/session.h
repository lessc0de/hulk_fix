
#ifndef _hulk_fix_session_h_
#define _hulk_fix_session_h_

#include "hulk/fix/message.h"

#include <stdint.h>

namespace hulk {
namespace  fix {

class session
{
public:
    struct send_handler {
        virtual void send( const std::string& msg ) = 0;
    };

    struct recv_handler {
        virtual void recv( const fields& msg ) = 0;
    };

    session( const value& protcol, const fields& header, send_handler&, recv_handler& );

    void send( const value& msg_type, const fields& msg );
    void recv( const fields& msg );

private:
    value _protocol;
    fields _header;

    uint64_t _seq_in;
    uint64_t _seq_out;

    send_handler& _send_handler;
    recv_handler& _recv_handler;
};

}
}

#endif
