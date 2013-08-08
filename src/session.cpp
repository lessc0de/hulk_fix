
#include "hulk/fix/session.h"

#include <sstream>

using namespace hulk::fix;

#define BEGIN_STRING 8
#define BODY_LENGTH 9
#define CHECK_SUM 10
#define MSG_TYPE 35
#define MSG_SEQ_NUM 34
#define DELIM '|'

session::session( const value& protocol, const fields& header, send_handler& sh, recv_handler& rh )
: _protocol( protocol ),
  _header( header ),
  _seq_in( 0 ),
  _seq_out( 0 ),
  _send_handler( sh ),
  _recv_handler( rh )
{
}

void
session::send( const value& msg_type, const fields& body )
{
    std::stringstream bodyStr;
    bodyStr << MSG_TYPE << "=" << msg_type << DELIM;
    bodyStr << MSG_SEQ_NUM << "=" << ++_seq_out << DELIM;

    for( fields::const_iterator it = _header.begin(); it != _header.end(); it++ ) {
        bodyStr << it->first << "=" << it->second << DELIM;
    }

    for( fields::const_iterator it = body.begin(); it != body.end(); it++ ) {
        bodyStr << it->first << "=" << it->second << DELIM;
    }

    int length = bodyStr.str().length();

    std::stringstream msgStr;
    msgStr << BEGIN_STRING << "=" << _protocol << DELIM;
    msgStr << BODY_LENGTH << "=" << length << DELIM;
    msgStr << bodyStr.str();
    msgStr << CHECK_SUM << "=000" << DELIM;

    _send_handler.send( msgStr.str() );
}

void
session::recv( const fields& msg )
{
    _recv_handler.recv( msg );
}
