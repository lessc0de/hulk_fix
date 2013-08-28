
#include "hulk/fix/session.h"
#include "hulk/fix/transport.h"
#include "hulk/core/logger.h"

#include <cstdio>
#include <ctime>
#include <sstream>

using namespace hulk;
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
core::log& log = core::logger::instance().get( "hulk.fix" );

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

session::session( transport& t )
: _transport( t ),
  _seq_in( 0 ),
  _seq_out( 0 )
{
    LOG_INFO( log, "created session @ " << this );
    _transport.set_session( *this );
}

session::session( const value& protocol, const fields& header, transport& t )
: _protocol( protocol ),
  _header( header ),
  _transport( t ),
  _seq_in( 0 ),
  _seq_out( 0 )
{
    LOG_INFO( log, "created session @ " << this );
    _transport.set_session( *this );
}

session::~session()
{
    LOG_INFO( log, "deleted session @ " << this );
}

void session::recv( const fields& msg )
{
    LOG_INFO( log, "recvd " << msg.size() << " fields\n" );
}

// TODO
// encoding is a bit shit
// too reliant on string streams
void session::send( const value& msg_type, const fields& body )
{
    std::string send_time;
    time_t t = time( 0 );
    set_utc_time( send_time, &t );

    std::stringstream bodyStr;
    bodyStr << MSG_TYPE << "=" << msg_type << DELIM;
    bodyStr << MSG_SEQ_NUM << "=" << ++_seq_out << DELIM;
    bodyStr << SENDING_TIME << "=" << send_time << DELIM;

    for( int i=0; i<_header.size(); i++ ) {
        bodyStr << _header[i]._tag << "=" << _header[i]._value << DELIM;
    }

    for( int j=0; j<body.size(); j++ ) {
        bodyStr << body[j]._tag << "=" << body[j]._value << DELIM;
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

    s = msgStr.str();

    LOG_INFO( log, "sending: " << s.c_str() );

    _transport.send( s.c_str(), s.size() );
}
