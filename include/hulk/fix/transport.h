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
    friend class decoder;

    virtual ~transport();

    void recv( const fields& );
    void recv( const char*, size_t );

    bool attached();
    void attach( const shared_ptr< session >& );
    void detach();

    virtual void send( const char*, size_t ) {}
    virtual void close() {}     // user wants to close the transport
    virtual void closed();      // transport was closed

private:
    void operator()( const fields& msg, const std::string& buf );

    shared_ptr< session > _session;      // recvd messages will be passed to this session
    decoder _decoder;
};

}
}

#endif
