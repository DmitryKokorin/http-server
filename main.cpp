#include <thread>
#include <iostream>
#include <vector>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

#include "util.hpp"
#include "options.hpp"
#include "http.hpp"

namespace http_server {

void worker()
{
    while (true) {

        int socketfd = task_queue().pop();

        char recv_buff[1024];
        std::vector<char> request_buff;
        std::vector<char> response_buff;

        while (true) {

            int read_cnt = recv(socketfd, recv_buff,
                    sizeof(recv_buff), MSG_NOSIGNAL);

            if (-1 == read_cnt)
                break;

            if (0 == read_cnt)
                break;

            request_buff.insert(request_buff.end(),
                    recv_buff, recv_buff + read_cnt);

            if (process_http_request(request_buff, response_buff)) {

                ssize_t total_cnt = 0;
                ssize_t sent_cnt = 0;

                while (total_cnt != response_buff.size()) {

                    sent_cnt = send(socketfd, &response_buff[0],
                            response_buff.size(), MSG_NOSIGNAL);

                    if (sent_cnt != -1)
                        total_cnt += sent_cnt;
                }


                shutdown(socketfd, SHUT_RDWR);
            }
        }
    }
}

}


int main(int argc, char **argv)
{
    using namespace http_server;

    options::parse(argc, argv);
    daemonize();

    const options &opt = options::instance();

    std::vector<std::thread> threads;

    for (int i = 0; i < opt.thread_count; ++i) {

        std::thread t(worker);
        threads.push_back(std::move(t));
    }


    sockaddr_in listen_addr;
    memset(&listen_addr, 0, sizeof(listen_addr));
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_port = htons(opt.port);
    inet_pton(AF_INET, opt.host.c_str(), &listen_addr.sin_addr);

    int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == bind(listen_sock,
                   (const struct sockaddr*)&listen_addr,
                   sizeof(listen_addr))) {

        exit(EXIT_FAILURE);
    }

    if (-1 == listen(listen_sock, SOMAXCONN))
        exit(EXIT_FAILURE);

    for (;;) {


        sockaddr_in incoming_addr;
        socklen_t   socklen = sizeof(incoming_addr);

        int conn_socket = accept(listen_sock,
                (struct sockaddr*)&incoming_addr, &socklen);

        if (-1 == conn_socket)
            continue;

        task_queue().push(conn_socket);
    }

    exit(EXIT_SUCCESS);
}
