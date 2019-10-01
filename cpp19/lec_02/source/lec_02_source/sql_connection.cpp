#include "sql_connection.h"

void SQLConnection::ExecuteQuery(std::string_view query) {
  log_file_ << "Executing query: " << query << std::endl;

  // ...
}

// static
std::shared_ptr<SQLConnection> SQLConnection::Create(
    const std::string& db_uri) {
  SQLConnection* raw = new SQLConnection(db_uri);
  return std::shared_ptr<SQLConnection>(raw);
}

SQLConnection::SQLConnection(const std::string& db_uri)
    : log_file_("log_0000.txt") {
  if (log_file_.is_open()) {
    log_file_ << "Connection to " << db_uri << std::endl;
  }
}
