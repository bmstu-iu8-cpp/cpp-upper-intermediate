#include "file_storage.h"

FileStorage::FileStorage(const std::string& filename) : stream_(filename) {}

bool FileStorage::IsValid() const {
  return stream_.is_open() && !stream_.eof();
}

bool FileStorage::OnUserSave(const std::string& user) {
  if (!IsValid())
    return false;

  stream_ << user << std::endl;
  return true;
}
