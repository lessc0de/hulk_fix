
#ifndef _hulk_fix_session_factory_h_
#define _hulk_fix_session_factory_h_

#include "hulk/fix/session.h"

namespace hulk {
namespace  fix {

class session_factory
{
public:
    virtual session* create() = 0;
};

}
}

#endif
