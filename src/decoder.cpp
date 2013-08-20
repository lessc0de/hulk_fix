
#include "hulk/fix/decoder.h"

using namespace hulk::fix;

decoder::decoder()
{
    _field_state = TAG;
    _message_state = SEEK_HEADER;
}

void decoder::init_state()
{
    _tag.clear();
    _val.clear();
    _header.clear();
    _body.clear();
    _field_state = TAG;
    _message_state = SEEK_HEADER;
}

bool decoder::is_header( tag t )
{
    return( t ==  8 || t ==  9 || t == 34 ||
            t == 35 || t == 49 || t == 56 );
}
