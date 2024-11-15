#include "http.hpp"

#include <filesystem>

// input:  "path/to/file/index.html"
// return: "Content-Type: text/html\r\n"
std::string HTTP::Content_Type(const std::string& _filepath)
{
    std::string extension = std::filesystem::path(_filepath).extension().string();

    if(extension == ".html")
    {
        return std::string{"Content-Type: text/html\r\n"};
    }
    else if(extension == ".gif")
    {
        return std::string{"Content-Type: image/gif\r\n"};
    }
    else
    {
        return std::string{"Content-Type: text/plain;charset=UTF-8\r\n"};
    }
}

// input:  404 ((int)HTTP::StatusCode::NOT_FOUND)
// return: "404 Not Found"
std::string HTTP::Status_Code_String(const int _code)
{
    switch(_code)
    {
    case 200: return std::string{"200 OK"};
    case 404: return std::string{"404 Not Found"};
    case 501: return std::string{"501 Not Implemented"};

    default:  return std::string{"500 Internal Server Error"};
    }
}

// input: 420
// return: "Content-Length: 420\r\n"
std::string HTTP::Content_Length(const unsigned long _size)
{
    return std::string{"Content-Length: " + std::to_string(_size) + "\r\n"};
}

// return: "\r\n"
std::string HTTP::New_Line_Characters(void)
{
    return std::string{"\r\n"};
}