#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <ctime>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <thread>

using boost::asio::ip::tcp;

class Request {
 public:
  Request(tcp::socket& socket) {
    boost::system::error_code error;
    length_ = socket.read_some(boost::asio::buffer(data_), error);
  }

  std::string data() const {
    return std::string(data_.begin(), data_.begin() + length_);
  }

 private:
  boost::array<char, 1024> data_;
  size_t length_;
};

struct Storage {
  virtual ~Storage() = default;
  virtual void OnRequest(const Request&) = 0;
};

class FileStorage : public Storage {
 public:
  ~FileStorage() override = default;

  void OnRequest(const Request& request) override {
    std::lock_guard<std::mutex> lk(mutex_);
    file_ << request.data();
  }

  FileStorage(const std::string& filename) : file_(filename) {}

 private:
  std::ofstream file_;
  std::mutex mutex_;
};

class BufferedFileStorage : public Storage {
 public:
  ~BufferedFileStorage() override {
    is_stopped_ = true;
    cv_.notify_one();
    writer_.join();
  }

  void OnRequest(const Request& request) override {
    std::lock_guard<std::mutex> lk(mutex_);
    queue_.push(request.data());
    cv_.notify_one();
  }

  BufferedFileStorage(const std::string& filename)
      : file_(filename), writer_(&BufferedFileStorage::FileWriter, this) {}

 private:
  void FileWriter() {
    while (true) {
      std::unique_lock<std::mutex> lk(mutex_);
      while (queue_.empty() && !is_stopped_) {
        cv_.wait(lk);
      }

      if (is_stopped_) return;

      auto value = queue_.front();
      queue_.pop();
      lk.unlock();

      file_ << value;
      file_.flush();
    }
  }

  std::ofstream file_;
  std::atomic<bool> is_stopped_ = false;
  std::mutex mutex_;
  std::condition_variable cv_;
  std::queue<std::string> queue_;
  std::thread writer_;
};

void Handler(tcp::socket socket, Storage* storage) {
  Request request(socket);
  storage->OnRequest(request);
}

int main() {
  const int port = 8080;

  BufferedFileStorage storage("file.txt");

  try {
    boost::asio::io_service io_service;

    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));

    for (;;) {
      tcp::socket socket(io_service);

      acceptor.accept(socket);

      std::thread worker(Handler, std::move(socket), &storage);
      worker.detach();
    }
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
