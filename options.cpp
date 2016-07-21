#include "options.hpp"

#include <getopt.h>

namespace http_server {


options::options()
    : host("localhost")
    , port(80)
    , directory("./")
    , pid_filename("http_srever.pid")
{
}

const options &options::instance()
{
    static options o;
    return o;
}

void options::parse(int argc, char **argv)
{
    static struct option long_options[] = {
        {"host", required_argument, 0, 'h'}
    };

    while (true) {

        int option_index;

        int c = getopt_long(argc, argv, "h:",
                long_options, &option_index);


    }
}

}
