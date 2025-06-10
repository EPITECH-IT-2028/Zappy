#include <exception>
#include <iostream>
#include <string>
#include "Error.hpp"
#include "server/ServerCommunication.hpp"
#include "parser/Parser.hpp"

#define OK 0
#define KO 84

bool isHelpRequested(int argc, char** argv) {
  return argc == 2 && std::string(argv[1]) == "--help";
}

int main(int argc, char* argv[]) {
  try {
    gui::Config config;
    if (isHelpRequested(argc, argv)) {
      config.displayHelp();
      return OK;
    }
    config.parse(argc, argv);

    std::cout << "Parsed options: port: " << config.getOptionP()
              << " / host: " << config.getOptionH() << std::endl;
    Network::ServerCommunication server(8080);
    server.run();
    std::cout << "GUI application started successfully." << std::endl;
    return OK;
  } catch (const gui::Error& e) {
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
