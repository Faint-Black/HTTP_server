#pragma once

#include <vector>
#include <string>
#include <optional>
#include "http.hpp"

typedef struct
{
    std::string                path_to_file;
    std::string                http_version;
    std::vector<std::string>   mimetypes;
    enum HTTP::Methodtype      method;
    enum HTTP::Connectiontype  connection_status;
}
HTTP_Request;

extern std::optional<HTTP_Request> Parse_HTTP_Request(const std::string& _request);