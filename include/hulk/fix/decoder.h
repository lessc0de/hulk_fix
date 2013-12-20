
#ifndef _hulk_fix_decoder_h_
#define _hulk_fix_decoder_h_

#include "hulk/fix/message.h"
#include <cstdlib>
#include <cctype>

namespace hulk {
namespace  fix {

class decoder
{
public:
    decoder();

    template< class T > void decode( T& cb, const char* msg, size_t size );

private:
    template< class T > void on_field( T& cb );

    void init_state();

    enum field_state {
        TAG, VALUE
    };

    enum message_state {
        SEEK_HEADER, SEEK_CHECKSUM
    };

    field_state _field_state;
    message_state _message_state;

    std::string _tag;
    std::string _val;
    std::string _buf;

    fields _msg;
};

template< class TCallback >
void decoder::decode( TCallback& cb, const char* msg, size_t size )
{
    for( size_t i = 0; i < size; ++i )
    {
        char c = msg[i];

        if( _field_state == TAG )
        {
            if( c == '=' )
            {
                // got the tag now look for the value
                _field_state = VALUE;
            }
            else
            if( !isdigit( c ) )
            {
                // non numeric data should cause us to start looking for the tag afresh
                _tag.clear();
            }
            else
            {
                _tag.push_back( c );
            }
        }
        else
        {
            if( c == (char)1 )
            {
                // if we get a 1 assume we now have a complete fix field
                on_field( cb );

                // reset state so we start looking for next tag
                _field_state = TAG;
                _tag.clear();
                _val.clear();
            }
            else
            {
                _val.push_back( c );
            }
        }
    }
}

template< class TCallback >
void decoder::on_field( TCallback& cb )
{
    if( _tag.size() == 0 || _val.size() == 0 ) {
        return;
    }

    tag tag = atoi( _tag.c_str() );

    if( tag <= 0 ) {
        return;
    }

    _buf.append( _tag );
    _buf.append( "=" );
    _buf.append( _val );
    _buf.append( "\x1" );

    _msg.push_back( field( tag, _val ) );

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
            cb( _msg, _buf );
            init_state();
        }
    }
}

}
}

#endif
