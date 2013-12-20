
#include <hulk/fix/decoder.h>
#include <hulk/core/stopwatch.h>
#include <iostream>

using namespace hulk;

const int NUM_MSGS = 500000;

void build_message( std::string& str )
{
    str.append( "8=FIX.4.2" ); str.append( "\x1" );
    str.append( "1000=VALUE_1000" ); str.append( "\x1" );
    str.append( "1001=VALUE_1001" ); str.append( "\x1" );
    str.append( "1002=VALUE_1001" ); str.append( "\x1" );
    str.append( "1003=VALUE_1001" ); str.append( "\x1" );
    str.append( "1004=VALUE_1001" ); str.append( "\x1" );
    str.append( "1005=VALUE_1001" ); str.append( "\x1" );
    str.append( "1006=VALUE_1001" ); str.append( "\x1" );
    str.append( "1007=VALUE_1001" ); str.append( "\x1" );
    str.append( "1008=VALUE_1001" ); str.append( "\x1" );
    str.append( "1009=VALUE_1001" ); str.append( "\x1" );
    str.append( "10=000" ); str.append( "\x1" );
}

class decoder_callback
{
public:
    void operator()( const fix::fields& flds, const std::string& buf ) {}
};

int main( int argc, char** argv )
{
    std::string msg;
    build_message( msg );

    fix::decoder decoder;
    decoder_callback cb;

    stopwatch watch;
    watch.start();

    for( int i=0; i<NUM_MSGS; ++i ) {
        decoder.decode( cb, msg.c_str(), msg.size() );
    }

    float secs = watch.elapsed_s();
    float msecs = secs * 1000000.0f;
    std::cout << NUM_MSGS << " messages decoded in " << secs << " seconds. messages per second " << NUM_MSGS / secs << ". micros per message " << msecs / NUM_MSGS << std::endl;
}
