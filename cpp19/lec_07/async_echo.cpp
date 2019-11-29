#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>

using boost::asio::ip::tcp;

/// Пример 1. Простой асинхронный Echo сервак
void Echo(std::shared_ptr<tcp::socket> socket) {
  std::shared_ptr<std::vector<char>> data =
      std::make_shared<std::vector<char>>(1024);

  auto write_callback = [=](boost::system::error_code ec, size_t n) mutable {
    Echo(std::move(socket));
    data.reset();
  };

  auto read_callback = [=](boost::system::error_code ec, size_t n) mutable {
    if (!ec) {
      socket->async_write_some(boost::asio::buffer(*data, n), write_callback);
    }
  };

  socket->async_read_some(boost::asio::buffer(*data), read_callback);
}

void DoAccept(tcp::acceptor& acceptor, boost::asio::io_service& io_service) {
  std::shared_ptr<tcp::socket> socket =
      std::make_shared<tcp::socket>(io_service);
  auto callback = [socket, &acceptor,
                   &io_service](boost::system::error_code ec) {
    Echo(std::move(socket));
    DoAccept(acceptor, io_service);
  };
  acceptor.async_accept(*socket, callback);
}

int main(int argc, char* argv[]) {
  try {
    boost::asio::io_service io_service;
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 80));

    DoAccept(acceptor, io_service);
    // AsyncHttpServer s(io_service, std::atoi("80"));

    io_service.run();
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
