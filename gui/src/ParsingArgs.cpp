/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** ParsingArgs.cpp
*/

#include "ParsingArgs.hpp"
#include "Error.hpp"

bool gui::Config::parse(int argc, char* argv[])
{
  int countP = 0;
  int countH = 0;
  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];
    if (arg == "-p" && i + 1 < argc) {
      optionP = argv[i+=1];
      try {
        int port = std::stoi(optionP);
        if (port <= 0 || port > 65535) {
          return false;
        }
      } catch (const std::exception&) {
        return false;
      }
      countP++;
      if (countP > 1)
        return false;
    } else if (arg == "-h" && i + 1 < argc) {
      optionH = argv[i+=1];
      countH++;
      if (countH > 1)
        return false;
    } else
      return false;
  }
  return true;
}