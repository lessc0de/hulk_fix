
#include <hulk/fix/decoder.h>
#include <hulk/fix/session.h>

#include <iostream>
#include <sstream>

using namespace hulk;

struct my_publisher : public fix::session::publisher
{
    virtual void publish( const std::string& msg ) {
        std::cout << "publish: " << msg << std::endl;
    }
};

struct my_processor : public fix::session::processor
{
    virtual void process( const fix::fields& msg )
    {
        for( fix::fields::const_iterator it = msg.begin(); it != msg.end(); it++ ) {
            std::cout << "field " << it->first << "=" << it->second << std::endl;
        }
    }
};

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

int main( int argc, char** argv )
{
    const char* host = "localhost";
    const int port = 8000;

    my_publisher mpub;
    my_processor mprc;

    fix::fields flds;
    flds[49] = "SENDER";
    flds[56] = "TARGET";
    fix::session session( "FIX.4.4", flds, mpub, mprc );

    std::string nos;
    build_nos( nos );
    fix::decoder decoder( session );
    decoder.decode( nos.c_str(), nos.size() );
}
