
#ifndef _hulk_fix_encoder_h_
#define _hulk_fix_encoder_h_

#include "hulk/fix/message.h"
#include "stdint.h"

namespace hulk {
namespace  fix {

void encode( std::string& out, const fields& flds );
void encode( std::string& out, uint64_t seq, const value& protocol, const value& type, const fields& header, const fields& body );

}
}

#endif
