#include <thread>

#include <unistd.h>

#include "util.hpp"




int main()
{
    using namespace http_server;

    daemonize();

    while(true)
        usleep(100);

    return 0;
}
