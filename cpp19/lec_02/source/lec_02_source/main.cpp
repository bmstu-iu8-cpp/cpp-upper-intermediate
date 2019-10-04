#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>

#include <thread>

#include "file_storage.h"
#include "http_server.h"
#include "sql_connection.h"

void ThreadWorker(std::shared_ptr<SQLConnection> connection) {
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(100ms);
  connection->ExecuteQuery("SELECT* FROM Table WHERE group = 'iu8'");
  // ...
}

int main() {
  // Unique ptr sample.
  std::unique_ptr<FileStorage> file_storage =
      std::make_unique<FileStorage>("db.txt");

  HttpServer server(std::move(file_storage));

  std::cout << server.Save("user one") << std::endl;
  std::cout << server.Save("user two") << std::endl;
  std::cout << server.Save("user three") << std::endl;

  // Shared ptr example.
  std::shared_ptr<SQLConnection> sql_connection =
      SQLConnection::Create("postgresql://localost:5000/students");

  std::thread th1(ThreadWorker, sql_connection);
  std::thread th2(ThreadWorker, sql_connection);
  std::thread th3(ThreadWorker, sql_connection);
  std::thread th4(ThreadWorker, sql_connection);

  th1.join();
  th2.join();
  th3.join();
  th4.join();

  return 0;
}
