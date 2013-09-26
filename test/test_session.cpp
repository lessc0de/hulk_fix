
#include "hulk/fix/session.h"

#include <iostream>

using namespace hulk;

struct my_publisher : public fix::session::publisher
{
    virtual void publish( const std::string& msg ) {
        std::cout << "publish: " << msg << std::endl;
    }
};

struct my_processor : public fix::session::processor
{
    virtual void process( const fix::fields& msg ) {
        ;
    }
};

int main( int argc, char** argv )
{
    my_publisher mpub;
    my_processor mprc;

    fix::fields flds;
    flds[49] = "SENDER";
    flds[56] = "TARGET";
    fix::session session( "FIX.4.4", flds, mpub, mprc );

    fix::fields body;
    body[55] = "VOD.L";
    body[128] = "XLON";

    session.send( "D", body );
    session.send( "D", body );
}
