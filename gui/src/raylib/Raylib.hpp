#pragma once

#include <raylib.h>
#include "raylib-cpp.hpp"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define LOGO_DURATION_FRAMES 120

namespace Gui {
  enum GameScreen {
    LOGO,
    TITLE,
    GAMEPLAY,
    ENDING
  };

  class Raylib {
    public:
      Raylib();
      ~Raylib() = default;

      void run();

    private:
      GameScreen _currentScreen;
      int _framesCounter;
      raylib::Window _window;
  };
}  // namespace Gui
