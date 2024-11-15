#include "generate_response.hpp"

#include <memory>
#include <cstring>
#include <iostream>
#include <vector>
#include <fstream>

static Slice Append_To_Buffer(const Slice _buffer, const std::string& _string);
static Slice Append_To_Buffer(const Slice _buffer, const char* _string);
static Slice Append_To_Buffer(const Slice _buffer, const char _char);
static Slice Append_To_Buffer(const Slice _buffer, const Slice _data);
static Slice Read_File(const std::string& _filepath);

Slice Make_Response(const std::optional<HTTP_Request>& _request)
{
    Slice response{.ptr = nullptr, .size = 0};

    if(not _request.has_value())
    {
        response = Append_To_Buffer(response, "500 Internal Server Error\r\n");
        response = Append_To_Buffer(response, "Connection: close\r\n");
        response = Append_To_Buffer(response, "\r\n");
        return response;
    }

    const std::string canonical_filepath = std::string{SERVER_FILES_PATH} + _request.value().path_to_file;
    const Slice       file_body          = Read_File(canonical_filepath);
    const std::string status_code        = HTTP::Status_Code_String((int)HTTP::StatusCode::OK);
    const std::string connection_status  = "Connection: close\r\n";
    const std::string content_type       = HTTP::Content_Type(_request.value().path_to_file);
    const std::string content_len        = HTTP::Content_Length(file_body.size);

    response = Append_To_Buffer(response, "HTTP/1.1 ");
    response = Append_To_Buffer(response, status_code);
    response = Append_To_Buffer(response, content_type);
    response = Append_To_Buffer(response, content_len);
    response = Append_To_Buffer(response, connection_status);
    response = Append_To_Buffer(response, "\r\n");
    response = Append_To_Buffer(response, file_body);

    // dealloc file body (that was already copied into response)
    if(file_body.ptr != nullptr)
    {
        std::free(file_body.ptr);
    }

    return response;
}

static Slice Append_To_Buffer(const Slice _buffer, const std::string& _string)
{
    const size_t append_size = _string.size();
    const size_t offset = _buffer.size;
    const char* data = _string.c_str();

    Slice result{.ptr = nullptr, .size = append_size + _buffer.size};

    if(_buffer.size == 0 || _buffer.ptr == nullptr)
    {
        result.ptr = (uint8_t*)std::malloc(append_size);
    }
    else
    {
        result.ptr = (uint8_t*)std::realloc(_buffer.ptr, _buffer.size + append_size);
    }

    std::memcpy(result.ptr + offset, data, append_size);
    
    return result;
}
static Slice Append_To_Buffer(const Slice _buffer, const char* _string)
{
    const size_t append_size = std::strlen(_string);
    const size_t offset = _buffer.size;
    const char* data = _string;

    Slice result{.ptr = nullptr, .size = append_size + _buffer.size};

    if(_buffer.size == 0 || _buffer.ptr == nullptr)
    {
        result.ptr = (uint8_t*)std::malloc(append_size);
    }
    else
    {
        result.ptr = (uint8_t*)std::realloc(_buffer.ptr, _buffer.size + append_size);
    }

    std::memcpy(result.ptr + offset, data, append_size);
    
    return result;
}
static Slice Append_To_Buffer(const Slice _buffer, const Slice _data)
{
    const size_t append_size = _data.size;
    const size_t offset = _buffer.size;
    const char* data = (char*)_data.ptr;

    Slice result{.ptr = nullptr, .size = append_size + _buffer.size};

    if(_buffer.size == 0 || _buffer.ptr == nullptr)
    {
        result.ptr = (uint8_t*)std::malloc(append_size);
    }
    else
    {
        result.ptr = (uint8_t*)std::realloc(_buffer.ptr, _buffer.size + append_size);
    }

    std::memcpy(result.ptr + offset, data, append_size);

    return result;
}
static Slice Append_To_Buffer(const Slice _buffer, const char _char)
{
    const size_t append_size = sizeof(_char);
    const size_t offset = _buffer.size;
    const char* data = &_char;

    Slice result{.ptr = nullptr, .size = append_size + _buffer.size};

    if(_buffer.size == 0 || _buffer.ptr == nullptr)
    {
        result.ptr = (uint8_t*)std::malloc(append_size);
    }
    else
    {
        result.ptr = (uint8_t*)std::realloc(_buffer.ptr, _buffer.size + append_size);
    }

    std::memcpy(result.ptr + offset, data, append_size);
    
    return result;
}

// quick steal from chatGPT
static Slice Read_File(const std::string& _filepath)
{
    Slice result{.ptr = nullptr, .size = 0};

    // Open the file in binary mode
    std::ifstream file(_filepath, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Error: could not open file " << _filepath << std::endl;
        return (Slice){.ptr = nullptr, .size = 0};
    }

    // Get the size of the file
    std::streamsize filesize = file.tellg();
    file.seekg(0, std::ios::beg);

    // Allocate memory for the file content
    result.ptr = (unsigned char*)std::malloc(filesize);

    // Read the content of the file into the buffer
    if (not file.read((char*)result.ptr, filesize))
    {
        std::cerr << "Error: could not read file " << _filepath << std::endl;
    }

    // Close the file and clean up
    file.close();

    result.size = filesize;
    return result;
}
