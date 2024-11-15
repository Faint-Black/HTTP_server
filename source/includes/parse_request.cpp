#include "parse_request.hpp"

#include <iostream>
#include <string>
#include <vector>

static bool Is_Char_Whitespace(char _c);

std::optional<HTTP_Request> Parse_HTTP_Request(const std::string& _request)
{
    HTTP_Request result;
    result.path_to_file = "undefined";
    result.http_version = "undefined";
    result.mimetypes = {};
    result.method = HTTP::Methodtype::undef;
    result.connection_status = HTTP::Connectiontype::undef;

    std::vector<std::string> word_list;
    std::string buffer;
    for(const auto c : _request)
    {
        // end of word, add to the list then restart the process
        if(Is_Char_Whitespace(c) && (not buffer.empty()))
        {
            word_list.push_back(buffer);
            buffer.clear();
            continue;
        }
        if(not Is_Char_Whitespace(c))
        {
            buffer += c;
        }
    }

    std::string accepting_arguments;
    bool filepath_is_next = false;
    for(const auto& word : word_list)
    {
        std::cout << "[" << word << "]";

        if(word.back() == ':')
        {
            accepting_arguments = word;
            continue;
        }

        if(filepath_is_next)
        {
            if(word == "/")
            {
                result.path_to_file = "/index.html"; //default path
            }
            else
            {
                result.path_to_file = word;
            }
            filepath_is_next = false;
            continue;
        }
        if(word == "GET")        
        {
            result.method = HTTP::Methodtype::GET;
            filepath_is_next = true;
            continue;
        }
        if(word == "HEAD")       
        {
            result.method = HTTP::Methodtype::HEAD;
            continue;
        }
        if(word == "POST")       
        {
            result.method = HTTP::Methodtype::POST;
            continue;
        }
        if(word == "PUT")        
        {
            result.method = HTTP::Methodtype::PUT;
            continue;
        }
        if(word == "TRACE")      
        {
            result.method = HTTP::Methodtype::TRACE;
            continue;
        }
        if(word == "OPTIONS")    
        {
            result.method = HTTP::Methodtype::OPTIONS;
            continue;
        }
        if(word == "DELETE")     
        {
            result.method = HTTP::Methodtype::DELETE;
            continue;
        }
        if(word.find("HTTP/") != std::string::npos)
        {
            result.http_version = word;
            continue;
        }

        if(accepting_arguments == "Accept:")
        {
            result.mimetypes.push_back(word);
        }

        if(accepting_arguments == "Connection:")
        {
            if(word == "keep-alive") 
            {
                result.connection_status = HTTP::Connectiontype::KEEPALIVE;
                continue;
            }
            if(word == "close")      
            {
                result.connection_status = HTTP::Connectiontype::CLOSE;
                continue;
            }
        }
    }

    // basic error handling, return null std::option if one is detected
    if((result.path_to_file == "undefined")
    || (result.http_version == "undefined")
    || (result.mimetypes.empty())
    || (result.method == HTTP::Methodtype::undef))
    {
        return std::nullopt;
    }
    else
    {
        return result;
    }
}

static bool Is_Char_Whitespace(char _c)
{
    return ((_c == ',') || (_c == ' ') || (_c == '\0') || (_c == '\t') || (_c == '\r') || (_c == '\n'));
}
