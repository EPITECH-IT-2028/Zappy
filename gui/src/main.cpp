#include <iostream>
#include <ostream>
#include "raylib/Raylib.hpp"
#include "server/ServerCommunication.hpp"

#define OK 0
#define KO 84

int main(void) {
  // Gui::Raylib zappy;
  //
  // zappy.run();
  try {
    Network::ServerCommunication server(8080);
    server.run();
  } catch (const std::runtime_error &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return KO;
  }
  return OK;
}
