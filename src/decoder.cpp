
#include "hulk/fix/decoder.h"

using namespace hulk::fix;

decoder::decoder()
{
    init_state();
}

void decoder::init_state()
{
    _tag.clear();
    _val.clear();
    _buf.clear();
    _msg.clear();

    _field_state = TAG;
    _message_state = SEEK_HEADER;
}
