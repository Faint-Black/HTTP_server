#include <iostream>
#include <string>
#include <chrono>
#include <array>
#include <boost/asio.hpp>

#include "includes/http.hpp"
#include "includes/parse_request.hpp"
#include "includes/generate_response.hpp"

#define BUFFER_SIZE 1024
#define NO_FLAGS 0
#define DEBUG_LOG 1
#define SERVER 1

using namespace boost;

int main(void)
{
    char buffer[BUFFER_SIZE];

    boost::system::error_code connection_errorcode;
    asio::io_context          connection_context;
    asio::ip::tcp::resolver   connection_resolver(connection_context);
    asio::ip::tcp::endpoint   server_endpoint(asio::ip::make_address(SERVER_IP), SERVER_PORT);
    asio::ip::tcp::acceptor   server_acceptor(connection_context, server_endpoint);
    asio::ip::tcp::socket     server_socket(connection_context);

    while (true)
    {
        std::cout << "listening for connectors..." << std::endl;

        server_acceptor.accept(server_socket, connection_errorcode);
        if(connection_errorcode)
        {
            std::cerr << "failed to accept connection \n";
            std::cerr << "library error message: " << connection_errorcode.message() << "\n";
            return 0;
        }
        else
        {
            std::cout << "Success! Accepted one connection from: " << server_socket.remote_endpoint() << std::endl;
        }

        // step 1 - read request
        server_socket.receive(asio::buffer(buffer, BUFFER_SIZE));
        std::string request_string = buffer;
        std::cout << "\n<< full request string: >>\n";
        std::cout << request_string;

        // step 2 - parse request
        std::cout << "<< parsed words: >>\n";
        auto parsed_request = Parse_HTTP_Request(request_string);
        std::cout << "\n\n";
        std::cout << "<< parsed request: >>" << std::endl;
        if(parsed_request.has_value())
        {
            std::cout << "request method: " << (int)parsed_request.value().method << std::endl;
            std::cout << "request filepath: " << parsed_request.value().path_to_file << std::endl;
            std::cout << "request HTTP version: " << parsed_request.value().http_version << std::endl;
            std::cout << "request accepted mimetypes: ";
            for(const auto& mimetype : parsed_request.value().mimetypes)
            {
                std::cout << "\"" << mimetype << "\" ";
            }
        }
        else
        {
            std::cout << "Failed\n";
        }

        // step 3 - generate a response
        Slice response;
        response = Make_Response(parsed_request);

        // step 4 - send response back to client
        server_socket.send(asio::buffer(response.ptr, response.size+1));

        // cleanup
        if(response.ptr != nullptr)
        {
            std::free(response.ptr);
        }
       
        // step 5 - teardown
        std::cout << "\nconnection closing \n" << std::endl;
        server_socket.close();
    }

    return 0;
}