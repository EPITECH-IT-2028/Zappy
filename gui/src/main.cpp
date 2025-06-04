#include <csignal>
#include <iostream>
#include <ostream>
#include "raylib/Raylib.hpp"
#include "server/ServerCommunication.hpp"

#define OK 0
#define KO 84

static void signalHandler(const int sig) {
  (void)sig;
  std::cout << std::endl;
  std::cout << "Server shutdown requested." << std::endl;
  Network::ServerCommunication::requestShutdown();
}

int main(void) {
  // Gui::Raylib zappy;
  //
  // zappy.run();
  std::signal(SIGINT, signalHandler);
  std::signal(SIGTERM, signalHandler);
  try {
    Network::ServerCommunication server(8080);
    server.run();
  } catch (const std::runtime_error &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return KO;
  }
  return OK;
}
