
#ifndef _hulk_fix_message_h_
#define _hulk_fix_message_h_

#include <string>
#include <map>

namespace hulk {
namespace  fix {

typedef int tag;
typedef std::string value;
typedef std::map< tag, value > fields;

}
}

#endif
