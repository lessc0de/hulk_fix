
#include "hulk/fix/transport.h"

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

class my_session : public fix::session
{
public:
    my_session( const fix::value& protocol, const fix::fields& header, fix::transport& transport )
    : session( protocol, header, transport ) {}

    virtual void recv( const fix::fields& msg ) {
        std::cout << "recv: " << msg.size() << " fields" << std::endl;
    }
};

int main( int argc, char** argv )
{
    fix::fields flds;
    flds.push_back( fix::field( 49, "SENDER" ) );
    flds.push_back( fix::field( 56, "TARGET" ) );

    std::string nos;
    build_nos( nos );

    fix::transport transport;
    my_session session( "FIX.4.4", flds, transport );
    transport.set_session( session );
    transport.recv( nos.c_str(), nos.size() );
}
