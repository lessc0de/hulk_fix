
#include "hulk/fix/decoder.h"
#include "hulk/fix/session.h"

#include <cstdlib>

using namespace hulk::fix;

decoder::decoder( session& s )
: _session( s )
{
    init_state();
}

void
decoder::init_state()
{
    _tag_buf.clear();
    _value_buf.clear();
    _fields.clear();
    _field_state = TAG;
    _message_state = SEEK_HEADER;
}

void
decoder::decode( const char* msg, size_t size )
{
    for( size_t i=0; i<size; i++ )
    {
        char c = msg[i];

        if( _field_state == TAG && c == '=' )
        {
            // if we get an equals assume next character is a fix value
            _field_state = VALUE;
        }
        else
        if( c == (char)1 )
        {
            // if we get a 1 assume we now have a complete fix field
            on_field();

            // reset state so we start looking for next tag
            _field_state = TAG;
            _tag_buf.clear();
            _value_buf.clear();
        }
        else
        {
            _field_state == TAG ? _tag_buf.push_back( c ) : _value_buf.push_back( c );

            // todo
            // shouldn't allow tag / value to grow indefinately
            // throw an exception if they get too big
        }
    }
}

void
decoder::on_field()
{
    if( _tag_buf.size() == 0 || _value_buf.size() == 0 ) {
        return; // error
    }

    tag tag = atoi( _tag_buf.c_str() );

    if( tag <= 0 ) {
        return; // error
    }

    _fields[tag] = _value_buf;

    if( _message_state == SEEK_HEADER )
    {
        if( tag == 8 ) {
            _message_state = SEEK_CHECKSUM;
        }
    }
    else
    {
        if( tag == 10 )
        {
            _session.recv( _fields );
            init_state();
        }
    }
}
