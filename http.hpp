#pragma once

#include <vector>

namespace http_server {

bool process_http_request(std::vector<char> &request_buff,
                          std::vector<char> &response_buff);
}
