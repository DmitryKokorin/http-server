#include <thread>
#include <iostream>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "util.hpp"
#include "options.hpp"

void worker()
{
}


int main(int argc, char **argv)
{
    using namespace http_server;

    options::parse(argc, argv);
    daemonize();

    const options &opt = options::instance();
    
    sockaddr_in listen_addr = {0};
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_port = opt.port;
    inet_pton(AF_INET, opt.host.c_str(), &listen_addr.sin_addr);

    int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == bind(listen_sock, &addr, sizeof(addr)))
        exit(EXIT_FAILURE);

    if (-1 == listen(listen_sock, SOMAXCONN))
        exit(EXIT_FAILURE);

    for (;;) {


        sockaddr_in incoming_addr;

        int conn_socket = accept(listen_socket, &incomming_addr, sizeof(incomming_addr));

        if (-1 == conn_socket)
            continue;

        task_queue().push(conn_socket);
    }
#if 0
    for (;;) {

        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);

        if (-1 == nfds) {

            perror("epoll_pwait()");
            exit(EXIT_FAILURE);
        }

        for (n = 0; n < nfds; ++n) {

            if (events[n].data.fd == listen_socket) {

                conn_socket = accept(listen_socket, NULL, NULL);

                if (-1 == conn_socket) {

                    perror("accept");
                    exit(EXIT_FAILURE);
                }

                set_nonblocking(conn_socket);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = conn_socket;

                if (-1 == epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_socket,
                            &ev)) {

                    perror("epoll_ctl: conn_sock");
                    exit(EXIT_FAILURE);
                }

            } else {

                char buff[BUFF_SIZE];
                size_t size = recv(events[n].data.fd, buff, BUFF_SIZE-1,
                        MSG_DONTWAIT);

                buff[size] = '\0';

                if (0 == strcmp(buff, "OFF"))
                    exit(EXIT_SUCCESS);

                qsort(buff, size, sizeof(char), comp);
                send(events[n].data.fd, buff, size, MSG_NOSIGNAL);
            }
        }
    }
#endif
    exit(EXIT_SUCCESS);
}
