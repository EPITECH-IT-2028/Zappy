#include "Raylib.hpp"

gui::Raylib::Raylib(network::ServerCommunication& serverCommunication)
    : _window(SCREEN_WIDTH, SCREEN_HEIGHT, "Zappy"),
      _framesCounter(0),
      _currentScreen(GameScreen::LOGO),
      _serverCommunication(serverCommunication) {
  if (!IsWindowReady())
    throw std::runtime_error("Failed to initialize Raylib window");
}

void gui::Raylib::run() {
  _window.SetTargetFPS(60);

  while (!WindowShouldClose()) {
    switch (_currentScreen) {
      case GameScreen::LOGO: {
        _framesCounter++;
        if (_framesCounter > LOGO_DURATION_FRAMES)
          _currentScreen = GameScreen::TITLE;
      } break;
      case GameScreen::TITLE: {
        if (IsKeyPressed(KEY_ENTER))
          _currentScreen = GameScreen::GAMEPLAY;
      } break;
      case GameScreen::GAMEPLAY: {
        if (IsKeyPressed(KEY_ENTER))
          _currentScreen = GameScreen::ENDING;
      } break;
      case GameScreen::ENDING: {
        if (IsKeyPressed(KEY_ENTER))
          _currentScreen = GameScreen::TITLE;
      } break;
      default:
        break;
    }

    _window.BeginDrawing();
    _window.ClearBackground(RAYWHITE);

    switch (_currentScreen) {
      case GameScreen::LOGO:
        renderLogoScreen();
        break;
      case GameScreen::TITLE:
        renderTitleScreen();
        break;
      case GameScreen::GAMEPLAY:
        renderGameplayScreen();
        break;
      case GameScreen::ENDING:
        renderEndingScreen();
        break;
      default:
        break;
    }

    _window.EndDrawing();
  }
}

void gui::Raylib::renderLogoScreen() {
  DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
  DrawText("Fake loading... Please wait 2 seconds.", 230, 220, 20, GRAY);
}

void gui::Raylib::renderTitleScreen() {
  DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GREEN);
  DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
  DrawText("PRESS ENTER to JUMP to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);
}

void gui::Raylib::renderGameplayScreen() {
  DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, PURPLE);
  DrawText("GAMEPLAY SCREEN", 20, 20, 40, MAROON);
  DrawText("PRESS ENTER to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);
}

void gui::Raylib::renderEndingScreen() {
  DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLUE);
  DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
  DrawText("PRESS ENTER to JUMP to TITLE SCREEN", 130, 220, 20, DARKBLUE);
}
