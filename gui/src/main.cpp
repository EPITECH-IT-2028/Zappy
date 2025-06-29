#include <exception>
#include <iostream>
#include <string>
#include "parser/ArgsParser.hpp"
#include "gameEngine/GameEngine.hpp"
#include "server/ServerCommunication.hpp"

#define OK 0
#define KO 84

/**
 * @brief Checks if the help flag is requested by the user.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @return true if '--help' is provided as the only argument, false otherwise.
 */
bool isHelpRequested(int argc, char** argv) {
  return argc == 2 && std::string(argv[1]) == "--help";
}

/**
 * @brief Entry point for the Zappy GUI application.
 *
 * Parses command-line arguments, establishes a connection to the server, and starts the main application loop.
 * Handles errors and displays appropriate messages to the user.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @return int Exit code (0 for success, 84 for error).
 */
int main(int argc, char* argv[]) {
  try {
    parser::Config config;
    if (isHelpRequested(argc, argv)) {
      config.displayHelp();
      return OK;
    }
    config.parse(argc, argv);

    network::ServerCommunication serverCommunication(
        std::stoi(config.getOptionP()), config.getOptionH());
    if (!serverCommunication.connectToServer()) {
      std::cerr << "Error while connecting to server at " << config.getOptionH()
                << ":" << config.getOptionP() << std::endl;
      return KO;
    }
    gui::GameEngine app{serverCommunication};
    app.run();
    return OK;
  } catch (const std::invalid_argument& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    std::cerr << "Use --help for help." << std::endl;
    return KO;
  } catch (const std::exception& e) {
    std::cerr << "Unexpected error: " << e.what() << std::endl;
    return KO;
  } catch (...) {
    std::cerr << "Unknown error occurred." << std::endl;
    return KO;
  }
}
