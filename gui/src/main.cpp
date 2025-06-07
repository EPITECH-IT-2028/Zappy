/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** main.cpp
*/

#include <iostream>
#include <string>
#include "ParsingArgs.hpp"

#define OK 0
#define KO 84

int checkError(int ac) {
  if (ac != 5) {
    return KO;
  }
  return OK;
}

int help_flag(int ac, char **av) {
  std::string help = "-h";
  if (ac == 2 && help.compare(av[1]) == 0) {
    std::cout << "USAGE: ./zappy_gui [-p port] [-h host]\n"
              << "  -p port: specify the port to connect to the server "
                 "(default: 8080)\n"
              << "  -h host: specify the host to connect to the server "
                 "(default: localhost)\n"
              << "  -h: display this help message\n";
    return OK;
  }
  return KO;
}

int main(int argc, char *argv[]) {
  gui::Config config;
  if (help_flag(argc, argv) == OK)
    return OK;
  if (checkError(argc) == KO) {
    std::cerr << "Invalid arguments. Use -h for help." << std::endl;
    return KO;
  }
  if (!config.parse(argc, argv)) {
    std::cerr << "Error parsing arguments." << std::endl;
    return KO;
  }
  std::cout << "Parsed options: port: " << config.getOptionP()
            << " / host: " << config.getOptionH() << std::endl;
  std::cout << "GUI application started successfully." << std::endl;
  return OK;
}
