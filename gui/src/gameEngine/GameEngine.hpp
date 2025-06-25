#pragma once

#include <raylib.h>
#include "commandHandler/CommandHandler.hpp"
#include "entities/GameState.hpp"
#include "raylib-cpp.hpp"
#include "server/ServerCommunication.hpp"

#define LOGO_DURATION_FRAMES 120
#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

namespace gui {
  enum class Screen {
    LOGO,
    TITLE,
    GAMEPLAY,
    ENDING
  };

  class GameEngine {
    public:
      GameEngine(network::ServerCommunication &serverCommunication);
      ~GameEngine() = default;

      void run();

    private:
      raylib::Window _window;
      int _framesCounter;

      Screen _currentScreen;
      network::ServerCommunication &_serverCommunication;
      GameState _gameState;
      handlecommand::CommandHandler _commandHandler;

      void updateLogoScreen();
      void updateTitleScreen();
      void updateGameplayScreen();
      void updateEndingScreen();
      void processNetworkMessages();

      void renderLogoScreen();
      void renderTitleScreen();
      void renderGameplayScreen();
      void renderEndingScreen();
  };
}  // namespace gui
