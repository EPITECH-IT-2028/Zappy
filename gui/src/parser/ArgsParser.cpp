#include "ArgsParser.hpp"
#include <iostream>
#include <stdexcept>

/**
 * @brief Parses command-line arguments for the Zappy GUI application.
 *
 * Expects exactly 4 arguments: -p <port> -h <host> (in any order).
 * Throws std::invalid_argument or std::out_of_range on invalid input.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 */
void parser::Config::parse(int argc, char* argv[]) {
  int countP = 0;
  int countH = 0;

  if (argc != 5) {
    throw std::invalid_argument(
        "Invalid number of arguments. Expected 4 arguments, got: " +
        std::to_string(argc - 1) + ". Use --help for usage information.");
  }

  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];
    if (arg == "-p" && i + 1 < argc) {
      _optionP = argv[i += 1];
      try {
        int port = std::stoi(_optionP);
        if (port <= 0 || port > 65535) {
          throw std::out_of_range("Port must be between 1 and 65535, got: " +
                                  _optionP);
        }
      } catch (const std::invalid_argument&) {
        throw std::invalid_argument("Invalid port format: " + _optionP);
      } catch (const std::out_of_range&) {
        throw std::out_of_range("Port out of range: " + _optionP);
      }
      countP++;
      if (countP > 1) {
        throw std::invalid_argument(
            "Port option (-p) specified multiple times");
      }
    } else if (arg == "-h" && i + 1 < argc) {
      _optionH = argv[i += 1];
      if (_optionH.empty()) {
        throw std::invalid_argument("Host option (-h) cannot be empty");
      }
      countH++;
      if (countH > 1) {
        throw std::invalid_argument(
            "Host option (-h) specified multiple times");
      }
    } else {
      throw std::invalid_argument("Unknown option: " + arg +
                                  ". Use --help for usage information.");
    }
  }

  if (countP == 0) {
    throw std::invalid_argument("Missing required port option (-p)");
  }
  if (countH == 0) {
    throw std::invalid_argument("Missing required host option (-h)");
  }
}

/**
 * @brief Gets the port option value as a string.
 * @return The port value provided by the user.
 */
std::string parser::Config::getOptionP() const {
  return _optionP;
}

/**
 * @brief Gets the host option value as a string.
 * @return The host value provided by the user.
 */
std::string parser::Config::getOptionH() const {
  return _optionH;
}

/**
 * @brief Displays help information for the Zappy GUI command-line arguments.
 */
void parser::Config::displayHelp() const {
  std::cout << "USAGE: ./zappy_gui [-p port] [-h host]\n"
            << "  -p port: specify the port to connect to the server\n"
            << "  -h host: specify the host to connect to the server\n"
            << "  --help: display this help message\n";
}
