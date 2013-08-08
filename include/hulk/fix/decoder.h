
#ifndef _hulk_fix_decoder_h_
#define _hulk_fix_decoder_h_

#include "hulk/fix/message.h"

namespace hulk {
namespace  fix {

class session;

class decoder
{
public:
    decoder( session& session );
    void decode( const char* msg, size_t size );

private:
    void init_state();
    void on_field();

    enum field_state {
        TAG, VALUE
    };

    enum messager_state {
        SEEK_HEADER, SEEK_CHECKSUM
    };

    session& _session;
    messager_state _message_state;
    field_state _field_state;
    fields _fields;
    std::string _tag_buf;
    std::string _value_buf;
};

}
}

#endif

