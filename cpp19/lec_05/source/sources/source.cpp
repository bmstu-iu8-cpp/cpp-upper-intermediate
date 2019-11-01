#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <ctime>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

std::string make_http_message(const char* data, int len) {
  std::string ua(data, len);
  std::stringstream response_body;
  std::stringstream response;
  response_body << "<title>Test C++ HTTP Server</title>\n"
                << "<h1>Test page</h1>\n"
                << "<p>This is body of the test page...</p>\n"
                << "<h2>Request headers</h2>\n"
                << "<pre>" << ua << "</pre>\n"
                << "<em><small>Test C++ Http Server</small></em>\n";

  // Сормируем весь ответ вместе с заголовками
  response << "HTTP/1.1 200 OK\r\n"
           << "Version: HTTP/1.1\r\n"
           << "Content-Type: text/html; charset=utf-8\r\n"
           << "Content-Length: " << response_body.str().length() << "\r\n\r\n"
           << response_body.str();
  return response.str();
}

class Request {
 public:
  Request(tcp::socket& socket) {
    boost::system::error_code error;
    length_ = socket.read_some(boost::asio::buffer(data_), error);
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

  boost::system::error_code Send(tcp::socket& socket) {
    std::stringstream response;
    response << "HTTP/1.1 " << code_ << " " << Code() << "\r\n";

    for (auto&& [header_name, value] : headers_) {
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
  response
    .AddHeader("Content-Type", "text/html; charset=utf-8")
      .AddHeader("HeaderNAme", "120340120340132")
      .SetCode(200)
      .SetBody("OK!");

  response.Send(socket);
}

int main() {
  const int port = 8080;

  try {
    boost::asio::io_service io_service;

    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));

    for (;;) {
      tcp::socket socket(io_service);

      acceptor.accept(socket);

      std::thread worker(Handler, std::move(socket));
      worker.detach();
    }
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}