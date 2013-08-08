
#include <sstream>
#include <iostream>

int main( int argc, char** argv )
{
    std::stringstream ss;
    ss << 1;

    std::cout << "'" << ss.rdbuf()->pbase() << "'" << std::endl;
}