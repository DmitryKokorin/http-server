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
  : host("127.0.0.1")
  , port(80)
  , directory("./")
  , pid_file("./http_serever.pid")
  , thread_count(1)
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
        {"host",          required_argument, 0, 'h'},
        {"port",          required_argument, 0, 'p'},
        {"directory",     required_argument, 0, 'd'},
        {"pid_file",      required_argument, 0, 'f'},
        {"thread_count",  required_argument, 0, 't'},
        {0,               0,                 0, 0  }
    };

    while (true) {

        int option_index;

        int c = getopt_long(argc, argv, "h:p:d:f:t:",
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

        case 't':

            o.thread_count = atoi(optarg);
            break;

        default:

            print_usage();
            exit(EXIT_FAILURE);
        }
    }
}

}
