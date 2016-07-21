#include <thread>
#include <iostream>

#include <unistd.h>

#include "util.hpp"
#include "options.hpp"


int main(int argc, char **argv)
{
    using namespace http_server;

    options::parse(argc, argv);
    const options &o = options::instance();

    std::cout << o.host      << std::endl
              << o.port      << std::endl
              << o.directory <<  std::endl
              << o.pid_file  << std::endl;


    daemonize();

    while(true)
        usleep(100);

    return 0;
}
