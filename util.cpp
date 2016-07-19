#include "util.hpp"

#include <fstream>
#include <stdexcept>

#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#include "options.hpp"


namespace http_server {


void daemonize()
{
    pid_t pid = fork();

    if (pid < 0)
        throw std::runtime_error("fork() failed");

    if (pid == 0) {

        umask(0);
        chdir("/");
        setsid();

        close(STDIN_FILENO);
        close(STDIN_FILENO);
        close(STDIN_FILENO);
    }
    else {

        std::fstream fs(pid_filename, std::ios::out);
        fs << pid;
        fs.sync();
        exit(EXIT_SUCCESS);
    }
}

}
