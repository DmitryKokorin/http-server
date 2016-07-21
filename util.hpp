#pragma once

#include "threadsafe_queue.hpp"

namespace http_server {

void daemonize();

typedef int task_t;
typedef threadsafe_queue<task_t> task_queue_t;

task_queue_t &task_queue();

}
