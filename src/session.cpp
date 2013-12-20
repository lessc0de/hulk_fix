
#include "hulk/fix/session.h"
#include "hulk/fix/transport.h"
#include "hulk/fix/encoder.h"
#include "hulk/core/logger.h"

#include <cstdio>
#include <ctime>
#include <sstream>

using namespace hulk;
using namespace hulk::fix;

namespace
{
log& l = logger::instance().get( "hulk.fix" );
}

session::session() : //transport_callback(),
  _transport( 0 ),
  _seq_in( 0 ),
  _seq_out( 0 )
{
}

session::session( const value& protocol, const fields& header ) : //transport_callback(),
  _protocol( protocol ),
  _header( header ),
  _transport( 0 ),
  _seq_in( 0 ),
  _seq_out( 0 )
{
}

session::~session()
{
}

void session::set_protocol( const value& protocol )
{
    _protocol = protocol;
}

void session::set_header( const fields& header )
{
    _header = header;
}

void session::set_transport( transport* transport )
{
    _transport = transport;
}

void session::on_recv( const fields& msg, const std::string buf )
{
    LOG_INFO( l, "recvd " << msg.size() << " fields\n" );
}

void session::send( const value& msg_type, const fields& body, std::string* copy_buf )
{
    std::string msg;
    encode( msg, ++_seq_out, _protocol, msg_type, _header, body );

    if( copy_buf ) {
        *copy_buf = msg;
    }

    LOG_DEBUG( l, "sending: " << msg );

    if( _transport ) {
        _transport->send( msg.c_str(), msg.size() );
    } else {
        LOG_DEBUG( l, "no transport available" );
    }
}
