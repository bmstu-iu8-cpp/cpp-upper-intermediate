#include <boost/array.hpp>
#include <boost/asio.hpp>

#include <ctime>
#include <iostream>
#include <string>
#include <sstream>
#include <map>


using boost::asio::ip::tcp;

class Request {
 public:
  Request(tcp::socket& socket) {
    boost::system::error_code error;
    length_ = socket.read_some(boost::asio::buffer(data_), error);
    // Parsing data...
  }

 private:
  boost::array<char, 128> data_;
  size_t length_;
};

class Response {
 public:
  Response() {}

  Response& AddHeader(const std::string& name, std::string value) {
    headers_[name] = std::move(value);
    return *this;
  }

  Response& SetCode(int code) {
    code_ = code;
    return *this;
  }

  Response& SetBody(std::string body) {
    body_ = std::move(body);
    AddHeader("Content-Length", std::to_string(body_.size()));
    return *this;
  }

  boost::system::error_code SendTo(tcp::socket& socket) {
    std::stringstream response;
    response << "HTTP/1.1 " << code_ << " " << Code() << "\r\n";

    for (const auto& [header_name, value] : headers_) {
      response << header_name << ": " << value << "\r\n";
    }

    response << "\r\n" << body_;

    auto s = response.str();

    boost::system::error_code error;
    boost::asio::write(socket, boost::asio::buffer(s), error);
    return error;
  }

 private:
  const char* Code() {
    // TODO
    if (code_ == 200)
      return "OK";
    else
      return "";
  }

  std::map<std::string, std::string> headers_;
  int code_ = 0;
  std::string body_;
};

void Handler(tcp::socket socket) {
  Response response;
  response.AddHeader("Content-Type", "text/html; charset=utf-8")
      .AddHeader("HeaderName", "120340120340132")
      .SetCode(200)
      .SetBody("Hello world!");

  response.SendTo(socket);
}

int main() {
  const int port = 8080;

  try {
    boost::asio::io_service io_service;

    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));

    for (;;) {
      tcp::socket socket(io_service);
      std::cout << "waiting client..." << std::endl;
      acceptor.accept(socket);
      std::cout << "yes..." << std::endl;

      std::thread worker(Handler, std::move(socket));
      worker.detach();
    }
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
