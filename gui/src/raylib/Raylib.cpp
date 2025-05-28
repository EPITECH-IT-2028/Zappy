#include "Raylib.hpp"

Gui::Raylib::Raylib() : _currentScreen(LOGO), _framesCounter(0), _window(SCREEN_WIDTH, SCREEN_HEIGHT, "Zappy") {
}

void Gui::Raylib::run() {
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    switch (_currentScreen) {
      case LOGO: {
        _framesCounter++;
        if (_framesCounter > 120)
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

    BeginDrawing();
    ClearBackground(RAYWHITE);

    switch (_currentScreen) {
      case LOGO:
        DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
        DrawText("Fake loading... Please wait 2 seconds.", 230, 220, 20, GRAY);
        break;
      case TITLE:
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GREEN);
        DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
        DrawText("PRESS ENTER to JUMP to GAMEPLAY SCREEN", 120, 220, 20,
                 DARKGREEN);
        break;
      case GAMEPLAY:
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, PURPLE);
        DrawText("GAMEPLAY SCREEN", 20, 20, 40, MAROON);
        DrawText("PRESS ENTER to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);
        break;
      case ENDING:
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLUE);
        DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
        DrawText("PRESS ENTER to JUMP to TITLE SCREEN", 130, 220, 20,
                 DARKBLUE);
        break;
      default:
        break;
    }

    EndDrawing();
  }
}
