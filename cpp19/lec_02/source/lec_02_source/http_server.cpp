#include "http_server.h"

HttpServer::HttpServer(std::unique_ptr<Storage> storage)
    : storage_(std::move(storage)) {}

std::string HttpServer::Save(const std::string& user) {
  return storage_->OnUserSave(user) ? "ok" : "error";
}
