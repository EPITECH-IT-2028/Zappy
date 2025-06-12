#pragma once

#include <raylib.h>
#include "parser/CommandParser.hpp"
#include "raylib-cpp.hpp"
#include "server/ServerCommunication.hpp"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define LOGO_DURATION_FRAMES 120

namespace gui {
  enum class Screen {
    LOGO,
    TITLE,
    GAMEPLAY,
    ENDING
  };

  class GameEngine {
    public:
      GameEngine(network::ServerCommunication& serverCommunication);
      ~GameEngine() = default;

      const parser::MapSize& getMapSize() const;
      const parser::TimeUnit& getTimeUnit() const;
      const parser::TeamNames& getTeamNames() const;
      void setMapSize(const std::string& command);
      void setTimeUnit(const std::string& command);
      void setTeamNames(const std::string& command);

      void run();

    private:
      raylib::Window _window;
      int _framesCounter;

      Screen _currentScreen;
      network::ServerCommunication& _serverCommunication;

      parser::MapSize _mapSize = parser::MapSize(0, 0);
      parser::TimeUnit _timeUnit = parser::TimeUnit(0);
      parser::TeamNames _teamNames = parser::TeamNames(std::vector<std::string>());

      void updateLogoScreen();
      void updateTitleScreen();
      void updateGameplayScreen();
      void updateEndingScreen();

      void renderLogoScreen();
      void renderTitleScreen();
      void renderGameplayScreen();
      void renderEndingScreen();

      void processServerCommand(const std::string& command);
  };
}  // namespace gui
