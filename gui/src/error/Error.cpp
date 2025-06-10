#include "Error.hpp"
#include <stdlib.h>
#include <string>

gui::Error::Error(const std::string &message) : _msg(message) {
}

const char *gui::Error::what() const noexcept {
  return _msg.c_str();
}
