#include "Error.hpp"

namespace gui {
    Error::Error(const std::string &message) : _msg(message) {}

    const char *Error::what() const noexcept {
        return _msg.c_str();
    }
}

