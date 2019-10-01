#pragma once

#include <string>

struct Storage {
  virtual ~Storage() = default;
  virtual bool OnUserSave(const std::string& user) = 0;
};
