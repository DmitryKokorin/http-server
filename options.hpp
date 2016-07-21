#pragma once

#include <string>


namespace http_server {

struct options
{
    std::string host;
    int         port;
    std::string directory;

    std::string pid_file;
    int         thread_count;

    static options &instance();
    static void parse(int argc, char **argv);

private:

    options();

    options(const options &) = delete;
    options &operator=(const options &) = delete;
};


}
