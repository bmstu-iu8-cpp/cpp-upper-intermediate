#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string make_http_message(const char* data, int len)
{
    std::string ua(data, len);
    std::stringstream response_body;
    std::stringstream response;
    response_body << "<title>Test C++ HTTP Server</title>\n"
                    << "<h1>Test page</h1>\n"
                    << "<p>This is body of the test page...</p>\n"
                    << "<h2>Request headers</h2>\n"
                    << "<pre>" << ua << "</pre>\n"
                    << "<em><small>Test C++ Http Server</small></em>\n";
        
    // Формируем весь ответ вместе с заголовками
    response << "HTTP/1.1 200 OK\r\n"
        << "Version: HTTP/1.1\r\n"
        << "Content-Type: text/html; charset=utf-8\r\n"
        << "Content-Length: " << response_body.str().length()
        << "\r\n\r\n"
        << response_body.str();
    return response.str();
}

int main()
{
    try
    {
        // Any program that uses asio need to have at least one io_service object
        boost::asio::io_service io_service;

        // acceptor object needs to be created to listen for new connections
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 8000));

        for (;;)
        {
            // creates a socket
            tcp::socket socket(io_service);

            // wait and listen
            acceptor.accept(socket);

            boost::array<char, 128> buf;
            boost::system::error_code error;
            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            std::string message = make_http_message(buf.data(), len);

            boost::system::error_code ignored_error;
            boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}