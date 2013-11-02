
#ifndef _hulk_fix_transport_h_
#define _hulk_fix_transport_h_

#include "hulk/core/shared_ptr.h"
#include "hulk/fix/decoder.h"

namespace hulk {
namespace  fix {

class session;

// -----------------------------------------------------------------------------
class transport
{
public:
    virtual ~transport();

    void send( const fields& );
    void recv( const fields& );
    void recv( const char*, size_t );

    void attach( shared_ptr< session >& );
    void detach();

    virtual void send( const char*, size_t ) {}
    virtual void close() {}     // user wants to close the transport
    virtual void closed() {}    // transport was closed

    void operator()( const fields& msg, const std::string& buf );

private:
    shared_ptr< session > _session;      // recvd messages will be passed to this session
    decoder _decoder;
};

}
}

#endif
