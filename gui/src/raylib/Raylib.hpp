#pragma once

#include <raylib.h>
#include "commandHandler/CommandHandler.hpp"
#include "entities/GameState.hpp"
#include "raylib-cpp.hpp"
#include "server/ServerCommunication.hpp"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define LOGO_DURATION_FRAMES 120

namespace gui {
  enum class GameScreen {
    LOGO,
    TITLE,
    GAMEPLAY,
    ENDING
  };

  class Raylib {
    public:
      Raylib(network::ServerCommunication &serverCommunication);
      ~Raylib() = default;

      void run();

    private:
      raylib::Window _window;
      int _framesCounter;

      GameScreen _currentScreen;
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
