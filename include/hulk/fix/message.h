
#ifndef _hulk_fix_message_h_
#define _hulk_fix_message_h_

#include <sstream>
#include <vector>
#include <map>

namespace hulk {
namespace  fix {

typedef int tag;

typedef std::string value;

struct field
{
    field( tag t, const value& v )
    : _tag( t ), _value( v )
    {
    }
    
    field( tag t, int v )
    : _tag( t )
    {
        std::stringstream ss; ss << v;
        _value = ss.str();
    }

    tag _tag;
    value _value;
};

typedef std::vector< field > fields;

struct field_map : public std::map< tag, const value* >
{
    field_map( const fields& f )
    {
        for( int i=0; i<f.size(); i++ ) {
            (*this)[f[i]._tag] = &f[i]._value;
        }
    }
};

}
}

#endif
