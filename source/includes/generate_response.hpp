#pragma once

#include "parse_request.hpp"
#include "http.hpp"

extern Slice Make_Response(const std::optional<HTTP_Request>& _request);