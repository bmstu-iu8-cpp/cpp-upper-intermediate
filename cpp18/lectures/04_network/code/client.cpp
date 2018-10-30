#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    try {
        boost::asio::io_service io_service;

        tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"), 8000);

        tcp::socket socket(io_service, ep.protocol());
        socket.connect(ep);

        boost::array<char, 128> buf;
        boost::system::error_code error;

        socket.write_some(boost::asio::buffer("Boost.Asio client"));

        int len = 0;
        while (len = socket.read_some(boost::asio::buffer(buf), error)) {
            if (error == boost::asio::error::eof) {

                break;
            } else if (error) {
                throw boost::system::system_error(error);
            }

            std::cout.write(buf.data(), len);
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}