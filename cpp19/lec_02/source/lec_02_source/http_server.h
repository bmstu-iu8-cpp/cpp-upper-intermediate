#pragma once

#include "storage.h"

class HttpServer {
 public:
  HttpServer(std::unique_ptr<Storage> storage);

  std::string Save(const std::string& user);

 private:
  std::unique_ptr<Storage> storage_;
};
