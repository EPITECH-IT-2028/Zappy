#pragma once

#include <string>
#include "entities/GameState.hpp"

namespace handlecommand {
  class CommandHandler {
    public:
      CommandHandler(gui::GameState& gameState) : _gameState(gameState) {};

      void handleBct(const std::string& command);
      void handlePnw(const std::string& command);
      void handlePpo(const std::string& command);
      void handlePlv(const std::string& command);
      void handlePin(const std::string& command);
      void handleEnw(const std::string& command);
      void handleEbo(const std::string& command);
      void handleEdi(const std::string& command);
      void handlePdi(const std::string& command);
      void handlePic(const std::string& command);

    private:
      void removeEgg(int eggId);
      gui::GameState& _gameState;
  };
}  // namespace handlecommand
