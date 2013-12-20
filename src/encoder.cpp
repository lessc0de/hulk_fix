
#include "hulk/fix/encoder.h"
#include <cstdio>

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

void hulk::fix::encode( std::string& out, const fields& flds )
{
    std::stringstream str;
    for( int i=0; i<flds.size(); i++ ) {
        str << flds[i]._tag << "=" << flds[i]._value << DELIM;
    }

    out = str.str();
}

void hulk::fix::encode( std::string& out, uint64_t seq, const value& protocol, const value& type, const fields& header, const fields& body )
{
    std::string send_time;
    time_t t = time( 0 );
    set_utc_time( send_time, &t );

    std::stringstream body_str;
    body_str << MSG_TYPE << "=" << type << DELIM;
    body_str << MSG_SEQ_NUM << "=" << seq << DELIM;
    body_str << SENDING_TIME << "=" << send_time << DELIM;

    for( int i=0; i<header.size(); i++ ) {
        body_str << header[i]._tag << "=" << header[i]._value << DELIM;
    }

    for( int j=0; j<body.size(); j++ ) {
        body_str << body[j]._tag << "=" << body[j]._value << DELIM;
    }

    int length = body_str.str().length();

    std::stringstream msg_str;
    msg_str << BEGIN_STRING << "=" << protocol << DELIM;
    msg_str << BODY_LENGTH << "=" << length << DELIM;
    msg_str << body_str.str();

    int checksum = 0;
    std::string s = msg_str.str();
    for( int i=0; i<s.size(); i++ ) {
        checksum += (int)s[i];
    }

    char buf[4];
    sprintf( buf, "%03d", checksum % 256 );

    msg_str << CHECK_SUM << "=" << buf << DELIM;
    out = msg_str.str();
}