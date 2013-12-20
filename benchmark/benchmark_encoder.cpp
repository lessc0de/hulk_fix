
#include <hulk/fix/encoder.h>
#include <hulk/core/stopwatch.h>
#include <iostream>

using namespace hulk;

const int NUM_MSGS = 500000;

int main( int argc, char** argv )
{
    fix::fields header;
    fix::fields body;
    body.push_back( fix::field( 8, "FIX.4.4" ) );
    body.push_back( fix::field( 1000, "VALUE_1000" ) );
    body.push_back( fix::field( 1001, "VALUE_1001" ) );
    body.push_back( fix::field( 1002, "VALUE_1002" ) );
    body.push_back( fix::field( 1003, "VALUE_1003" ) );
    body.push_back( fix::field( 1004, "VALUE_1004" ) );
    body.push_back( fix::field( 1005, "VALUE_1005" ) );
    body.push_back( fix::field( 1006, "VALUE_1006" ) );
    body.push_back( fix::field( 1007, "VALUE_1007" ) );
    body.push_back( fix::field( 1008, "VALUE_1008" ) );
    body.push_back( fix::field( 1009, "VALUE_1009" ) );
    body.push_back( fix::field( 10, "000" ) );

    std::string out;

    stopwatch watch;
    watch.start();

    for( int i=0; i<NUM_MSGS; ++i ) {
        fix::encode( out, 1, "FIX.4.4", "D", header, body );
    }

    float secs = watch.elapsed_s();
    float msecs = secs * 1000000.0f;
    std::cout << NUM_MSGS << " messages encoded in " << secs << " seconds. messages per second " << NUM_MSGS / secs << ". micros per message " << msecs / NUM_MSGS << std::endl;
}
