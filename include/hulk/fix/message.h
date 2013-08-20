
#ifndef _hulk_fix_message_h_
#define _hulk_fix_message_h_

#include <string>
#include <vector>

namespace hulk {
namespace  fix {

typedef int tag;

typedef std::string value;

struct field
{
    field( tag t, const value& v ) : _tag( t ), _value( v ) {}
    tag _tag;
    value _value;
};

typedef std::vector< field > fields;

}
}

#endif
