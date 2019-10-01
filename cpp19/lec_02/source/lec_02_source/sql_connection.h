#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>

class SQLConnection {
 public:
  // Not thread-safe.
  void ExecuteQuery(std::string_view query);

  static std::shared_ptr<SQLConnection> Create(const std::string& db_uri);

 private:
  SQLConnection(const std::string& db_uri);

  std::ofstream log_file_;
};
