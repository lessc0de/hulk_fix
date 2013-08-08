#include "hulk/core/tcp.h"
#include "hulk/core/logger.h"

#include <string>
#include <sstream>
#include <iostream>
#include <unistd.h>

using namespace hulk::core;

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

struct client_callback : public tcp_loop::callback
{
    virtual void on_connect( int fd )
    {
        LOG_INFO( "client: " << fd << " connected" );

        std::string nos;
        build_nos( nos );

        while( 1 ) {
            write( fd, nos.c_str(), nos.size() );
        }
    }

    virtual void on_disconnect( int fd ) {
        LOG_INFO( "client: " << fd << " disconnected" );
    }

    virtual void on_recv( int fd, const char* data, size_t len ) {
        ;
    }
};

int main()
{
    const char* host = "localhost";
    const int port = 8000;

    client_callback client_cb;
    tcp_loop loop;
    loop.tcp_client( "localhost", port, client_cb );
    //loop.start();
}
