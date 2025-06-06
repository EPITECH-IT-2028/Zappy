#include <iostream>
#include <ostream>
// #include "raylib/Raylib.hpp"
#include "server/ServerCommunication.hpp"

#define OK 0
#define KO 84

int main(int argc, char **argv) {
  // Gui::Raylib zappy;
  //
  // zappy.run();
  int port = (argc > 1) ? std::atoi(argv[1]) : 8080;

  try {
    Network::ClientCommunication client(port);
    if (!client.connectToServer()) {
      std::cerr << "Failed to connect to server." << std::endl;
      return KO;
    }
  } catch (const std::runtime_error &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return KO;
  }
  return OK;
}
