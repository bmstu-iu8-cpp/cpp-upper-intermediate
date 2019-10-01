#pragma once

#include <fstream>
#include <string>

#include "storage.h"

class FileStorage : public Storage {
 public:
  FileStorage(const std::string& filename);
  ~FileStorage() = default;

  bool OnUserSave(const std::string& user) override;

  bool IsValid() const;

 private:
  std::fstream stream_;
};
