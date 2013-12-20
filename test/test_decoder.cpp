
#include <hulk/fix/decoder.h>
#include <cassert>

using namespace hulk;

size_t num_decoded = 0;
std::string msg;

void build_message( std::string& str )
{
    str.append( "8=FIX.4.2" ); str.append( "\x1" );
    str.append( "58=TEST" ); str.append( "\x1" );
    str.append( "10=000" ); str.append( "\x1" );
}

class decoder_callback
{
public:
    void operator()( const fix::fields& flds, const std::string& buf )
    {
        fix::field_map fmap( flds );
        assert( *fmap[8]  == "FIX.4.2" );
        assert( *fmap[58] == "TEST" );
        assert( *fmap[10] == "000" );
        assert( msg == buf );
        assert( 3 == flds.size() );

        ++num_decoded;
    }
};

int main( int argc, char** argv )
{
    std::string garbage1( "\x1\x1\x1" );
    std::string garbage2( "..." );

    build_message( msg );

    fix::decoder decoder;

    decoder_callback cb;
    decoder.decode( cb, garbage1.c_str(), garbage1.size() );
    decoder.decode( cb, msg.c_str(), msg.size() );
    decoder.decode( cb, msg.c_str(), msg.size() );
    decoder.decode( cb, garbage1.c_str(), garbage1.size() );
    decoder.decode( cb, msg.c_str(), msg.size() );
    decoder.decode( cb, garbage2.c_str(), garbage2.size() );
    decoder.decode( cb, msg.c_str(), msg.size() );
    decoder.decode( cb, garbage1.c_str(), garbage1.size() );
    decoder.decode( cb, msg.c_str(), msg.size() );
    decoder.decode( cb, msg.c_str(), msg.size() );

    assert( num_decoded == 6 );
}
