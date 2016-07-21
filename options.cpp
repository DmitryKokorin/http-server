#include "options.hpp"

#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>

namespace http_server {

namespace {

void print_usage()
{
    //TODO: some code here
}

}


options::options()
  : host("localhost")
  , port(80)
  , directory("./")
  , pid_file("./http_serever.pid")
{
}

options &options::instance()
{
    static options o;
    return o;
}

void options::parse(int argc, char **argv)
{
    options &o = instance();

    static struct option long_options[] = {
        {"host",      required_argument, 0, 'h'},
        {"port",      required_argument, 0, 'p'},
        {"directory", required_argument, 0, 'd'},
        {"pid_file",  required_argument, 0, 'f'},
        {0,           0,                 0, 0  }
    };

    while (true) {

        int option_index;

        int c = getopt_long(argc, argv, "h:p:d:f:",
                long_options, &option_index);

        if (-1 == c)
            break;

        switch (c) {

        case 'h':

            o.host = optarg;
            break;

        case 'p':

            o.port = atoi(optarg);
            break;

        case 'd':

            o.directory = optarg;
            break;

        case 'f':

            o.pid_file = optarg;
            break;

        default:

            print_usage();
            exit(EXIT_FAILURE);
        }
    }
}

}
