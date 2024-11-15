#pragma once

#include <string>

#define SERVER_MAX_RETRIES 5
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define SERVER_FILES_PATH "../server-files"

typedef struct
{
    unsigned char* ptr;
    unsigned long size;
}Slice;


namespace HTTP
{
    enum class StatusCode : int
    {
        OK              = 200,
        NOT_FOUND       = 404,
        NOT_IMPLEMENTED = 501,
    };
    enum class Methodtype
    {
        undef,
        GET,
        HEAD,
        POST,
        PUT,
        TRACE,
        OPTIONS,
        DELETE,
    };
    enum class Connectiontype
    {
        undef,
        KEEPALIVE,
        CLOSE,
    };

    extern std::string Content_Type(const std::string& _filepath);
    extern std::string Status_Code_String(const int _code);
    extern std::string Content_Length(const unsigned long _size);
    extern std::string New_Line_Characters(void);
};