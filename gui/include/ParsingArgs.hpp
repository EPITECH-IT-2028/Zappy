/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** ParsingArgs.hpp
*/

#include <string>

#ifndef PARSINGARGS_HPP_
  #define PARSINGARGS_HPP_

namespace gui {
  class Config {
    public:
      bool parse(int argc, char* argv[]);

      std::string getOptionP() const { return optionP; }
      std::string getOptionH() const { return optionH; }

    private:
      std::string optionP;
      std::string optionH;
  };
}  // namespace gui

#endif /* PARSINGARGS_HPP_ */