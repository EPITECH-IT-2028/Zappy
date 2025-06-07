/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** main.cpp
*/

#include <iostream>
#include <string>
#include "Macro.hpp"
#include "ParsingArgs.hpp"

int checkError(int ac)
{
  if (ac != 5) {
    return ERROR;
  }
  return SUCCESS;
}

int help_flag(int ac, char **av)
{
  std::string help = "-h";
  if (ac == 2 && help.compare(av[1]) == 0) {
    std::cout << "USAGE: ./zappy_gui -p port -h machine" << std::endl;
    return SUCCESS;
  }
  return ERROR;
}

int main(int argc, char* argv[])
{
  gui::Config config;
  if (help_flag(argc, argv) == SUCCESS)
    return SUCCESS;
  if (checkError(argc) == ERROR) {
    std::cerr << "Invalid arguments. Use -h for help." << std::endl;
    return ERROR;
  }
  if (!config.parse(argc, argv)) {
    std::cerr << "Error parsing arguments." << std::endl;
    return ERROR;
  }
  std::cout << "Parsed options: port: " << config.getOptionP() << " / host: " << config.getOptionH() << std::endl;
  std::cout << "GUI application started successfully." << std::endl;  
  return SUCCESS;
}
