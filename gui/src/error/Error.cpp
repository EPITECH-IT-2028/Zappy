/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** check_error.cpp
*/

#include "Error.hpp"
#include <stdlib.h>
#include <string>

nts::Error::Error(const std::string &message) : _msg(message) {
}

const char *nts::Error::what() const noexcept {
  return _msg.c_str();
}
