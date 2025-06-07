#include "Raylib.hpp"

Gui::Raylib::Raylib()
    : _window(SCREEN_WIDTH, SCREEN_HEIGHT, "Zappy"),
      _framesCounter(0),
      _currentScreen(LOGO) {
  if (!IsWindowReady())
    throw std::runtime_error("Failed to initialize Raylib window");
}

void Gui::Raylib::run() {
  _window.SetTargetFPS(60);

  while (!WindowShouldClose()) {
    switch (_currentScreen) {
      case LOGO: {
        _framesCounter++;
        if (_framesCounter > LOGO_DURATION_FRAMES)
          _currentScreen = TITLE;
      } break;
      case TITLE: {
        if (IsKeyPressed(KEY_ENTER))
          _currentScreen = GAMEPLAY;
      } break;
      case GAMEPLAY: {
        if (IsKeyPressed(KEY_ENTER))
          _currentScreen = ENDING;
      } break;
      case ENDING: {
        if (IsKeyPressed(KEY_ENTER))
          _currentScreen = TITLE;
      } break;
      default:
        break;
    }

    _window.BeginDrawing();
    _window.ClearBackground(RAYWHITE);

    switch (_currentScreen) {
      case LOGO:
        renderLogoScreen();
        break;
      case TITLE:
        renderTitleScreen();
        break;
      case GAMEPLAY:
        renderGameplayScreen();
        break;
      case ENDING:
        renderEndingScreen();
        break;
      default:
        break;
    }

    _window.EndDrawing();
  }
}

void Gui::Raylib::renderLogoScreen() {
  DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
  DrawText("Fake loading... Please wait 2 seconds.", 230, 220, 20, GRAY);
}

void Gui::Raylib::renderTitleScreen() {
  DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GREEN);
  DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
  DrawText("PRESS ENTER to JUMP to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);
}

void Gui::Raylib::renderGameplayScreen() {
  DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, PURPLE);
  DrawText("GAMEPLAY SCREEN", 20, 20, 40, MAROON);
  DrawText("PRESS ENTER to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);
}

void Gui::Raylib::renderEndingScreen() {
  DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLUE);
  DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
  DrawText("PRESS ENTER to JUMP to TITLE SCREEN", 130, 220, 20, DARKBLUE);
}
