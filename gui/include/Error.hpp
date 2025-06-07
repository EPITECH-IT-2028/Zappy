/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** Error.hpp
*/

#include <exception>
#include <string>

#ifndef ERROR_HPP_
  #define ERROR_HPP_

namespace gui {
  class Error : public std::exception {
    public:
      Error(const std::string &message);
      ~Error() = default;

      const char *what() const noexcept override;

    private:
      std::string _msg;
  };
}  // namespace gui

#endif /* ERROR_HPP_ */
