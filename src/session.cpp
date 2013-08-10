
#include "hulk/fix/session.h"

#include <cstdio>
#include <ctime>
#include <sstream>

using namespace hulk::fix;

#define BEGIN_STRING 8
#define BODY_LENGTH 9
#define CHECK_SUM 10
#define MSG_TYPE 35
#define MSG_SEQ_NUM 34
#define SENDING_TIME 52
#define DELIM (char)1

namespace
{
void set_utc_time( std::string& s, time_t* t )
{
    struct tm* utc = gmtime( t );

    char buf[32];
    sprintf( buf, "%04d%02d%02d-%02d:%02d:%02d",
        utc->tm_year+1900,
        utc->tm_mon+1,
        utc->tm_mday,
        utc->tm_hour,
        utc->tm_min,
        utc->tm_sec );

    s = buf;
}
}

session::session( const value& protocol, const fields& header, send_handler& sh, recv_handler& rh )
: _protocol( protocol ),
  _header( header ),
  _seq_in( 0 ),
  _seq_out( 0 ),
  _send_handler( sh ),
  _recv_handler( rh )
{
}

void session::send( const value& msg_type, const fields& body )
{
    std::string send_time;
    time_t t = time( 0 );
    set_utc_time( send_time, &t );

    std::stringstream bodyStr;
    bodyStr << MSG_TYPE << "=" << msg_type << DELIM;
    bodyStr << MSG_SEQ_NUM << "=" << ++_seq_out << DELIM;
    bodyStr << SENDING_TIME << "=" << send_time << DELIM;

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

    int checksum = 0;
    std::string s = msgStr.str();
    for( int i=0; i<s.size(); i++ ) {
        checksum += (int)s[i];
    }

    char buf[4];
    sprintf( buf, "%03d", checksum % 256 );

    msgStr << CHECK_SUM << "=" << buf << DELIM;

    _send_handler.send( msgStr.str() );
}

void session::recv( const fields& msg )
{
    _recv_handler.recv( msg );
}
