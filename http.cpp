#include "http.hpp"

#include <string>
#include <map>
#include <fstream>

#include <unistd.h>

#include "http.def"

namespace http_server {

using namespace std;

namespace {

vector<string> split(const string &str, const string &sep)
{
    vector<string> tokens;

    size_t token_begin = 0;
    size_t token_end;
    size_t find_idx;

    while (true) {

        find_idx = str.find(sep, token_begin);

        token_end = find_idx == string::npos ? str.size() - 1 : find_idx;
        tokens.push_back(str.substr(token_begin, token_end - token_begin));

        if (find_idx == string::npos)
            break;

        token_begin = token_end + sep.size();
    }

    return tokens;
}

#define X(a, b, c) b, 
enum {
    HTTP_CODES_TABLE
};
#undef X

struct http_status_t
{
    int code;
    string description;
};

#define X(a, b, c) {a, c},
http_status_t http_status[] = {
    HTTP_CODES_TABLE
};
#undef X

}

bool process_http_request(vector<char> &request_buff,
                          vector<char> &response_buff)
{
    using namespace std;

    string request(&request_buff[0], request_buff.size());

    size_t pos = request.find("\r\n\r\n");
    if (string::npos == pos)
        return false;

    request.erase(pos, request.size() - pos);

    vector<string> tokens = split(request, "\r\n");

    if (tokens.empty())
        return false;

    vector<string> start_str_tokens = split(tokens[0], " ");

    string response;

    int status;
    string headers;
    string data;

    if (  start_str_tokens.empty()
       || start_str_tokens.size() != 3
       || start_str_tokens.front() != "GET") {

        status = NOT_IMPLEMENTED;
    }
    else {

        string url = start_str_tokens[1];

        if (!url.empty() && url.front() == '/')
            url = url.substr(1, url.size() - 1);

        if (url.empty())
            url = "index.html";

        size_t pos = url.find('?');
        if (string::npos != pos)
            url = url.substr(0, pos);

        if (F_OK != access(url.c_str(), F_OK)) {

            status = NOT_FOUND;
        }
        else if (F_OK != access(url.c_str(), R_OK)) {

            status = FORBIDDEN;
        }
        else {

            ifstream ifs(url);

            ifs.seekg(0, ios::end);
            streampos length = ifs.tellg();
            ifs.seekg(0, ios::beg);

            vector<char> buffer(length);
            ifs.read(&buffer[0], length);

            data.assign(begin(buffer), end(buffer));

            //data.assign(istreambuf_iterator<char>(ifs),
            //             istreambuf_iterator<char>());

            headers = string()
                + "Content-Type: text/html\r\n"
                + "Content-Length: " + to_string(data.size()) + "\r\n";

            status = OK;
        }
    }


    response = "HTTP/1.0 " 
        + to_string(http_status[status].code)
        + ' '
        + http_status[status].description
        + "\r\n"
        + headers
        + "\r\n"
        + data;

    response_buff = vector<char>(begin(response), end(response));

    return true;
}

}
