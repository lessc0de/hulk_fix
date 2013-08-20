
#include <hulk/fix/decoder.h>
#include <hulk/fix/session.h>

#include <iostream>
#include <sstream>

using namespace hulk;

void
build_nos( std::string& str )
{
    std::stringstream body;
    body << "8=FIX.4.2" << (char)1
         << "9=0190" << (char)1
         << "35=D" << (char)1
         << "49=CGML" << (char)1
         << "56=CHIX" << (char)1
         << "52=20121011-11:10:11.048" << (char)1
         << "34=457227" << (char)1
         << "48=JE00B2R84W06" << (char)1
         << "207=L" << (char)1
         << "15=GBX" << (char)1
         << "1=0004" << (char)1
         << "11=920000019793712" << (char)1
         << "38=65" << (char)1
         << "40=2" << (char)1
         << "44=731.5" << (char)1
         << "60=20121011-11:10:11.015" << (char)1
         << "54=1" << (char)1
         << "47=P" << (char)1
         << "59=3" << (char)1
         << "9303=B" << (char)1
         << "9479=X" << (char)1
         << "10=025" << (char)1;

    str = body.str();
}

void print_fields( const fix::fields& f )
{
    for( int i=0; i<f.size(); i++ ) {
        std::cout << f[i]._tag << " = " << f[i]._value << std::endl;
    }
}

class decoder_callback
{
public:
    void operator()( const fix::fields& header, const fix::fields& body )
    {
        std::cout << "\nheader\n";
        print_fields( header );

        std::cout << "\nbody\n";
        print_fields( body );
    }
};

int main( int argc, char** argv )
{
    std::string nos;
    build_nos( nos );

    fix::decoder decoder;
    decoder_callback cb;
    decoder.decode( cb, nos.c_str(), nos.size() );
}
