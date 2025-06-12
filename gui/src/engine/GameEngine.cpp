#include "GameEngine.hpp"

gui::GameEngine::GameEngine(network::ServerCommunication& serverCommunication)
    : _window(SCREEN_WIDTH, SCREEN_HEIGHT, "Zappy"),
      _framesCounter(0),
      _currentScreen(Screen::LOGO),
      _serverCommunication(serverCommunication) {
  if (!IsWindowReady())
    throw std::runtime_error("Failed to initialize Raylib window");
}

const parser::MapSize& gui::GameEngine::getMapSize() const {
  return _mapSize;
}

const parser::TimeUnit& gui::GameEngine::getTimeUnit() const {
  return _timeUnit;
}

const parser::TeamNames& gui::GameEngine::getTeamNames() const {
  return _teamNames;
}

void gui::GameEngine::setMapSize(const std::string& command) {
  _mapSize = parser::CommandParser::parseMsz(command);
}

void gui::GameEngine::setTimeUnit(const std::string& command) {
  _timeUnit = parser::CommandParser::parseSgt(command);
}

void gui::GameEngine::setTeamNames(const std::string& command) {
  _teamNames = parser::CommandParser::parseTna(command);
}

void gui::GameEngine::run() {
  _window.SetTargetFPS(60);

  while (!WindowShouldClose()) {
    switch (_currentScreen) {
      case Screen::LOGO:
        updateLogoScreen();
        break;
      case Screen::TITLE:
        updateTitleScreen();
        break;
      case Screen::GAMEPLAY:
        updateGameplayScreen();
        break;
      case Screen::ENDING:
        updateEndingScreen();
        break;
      default:
        break;
    }

    _window.BeginDrawing();
    _window.ClearBackground(RAYWHITE);

    switch (_currentScreen) {
      case Screen::LOGO:
        renderLogoScreen();
        break;
      case Screen::TITLE:
        renderTitleScreen();
        break;
      case Screen::GAMEPLAY:
        renderGameplayScreen();
        break;
      case Screen::ENDING:
        renderEndingScreen();
        break;
      default:
        break;
    }

    _window.EndDrawing();
  }
}

void gui::GameEngine::updateLogoScreen() {
  _framesCounter++;
  if (_framesCounter > LOGO_DURATION_FRAMES) {
    _currentScreen = Screen::TITLE;
    _framesCounter = 0;
  }
}

void gui::GameEngine::updateTitleScreen() {
  if (IsKeyPressed(KEY_ENTER)) {
    processServerCommand("msz 10 10");
    processServerCommand("sgt 126");
    processServerCommand("tna team1 team2 team3");
    _currentScreen = Screen::GAMEPLAY;
  }
}

void gui::GameEngine::updateGameplayScreen() {
  // std::string serverCommand = _serverCommunication.receiveCommand();
  // if (!serverCommand.empty())
  //   processServerCommand(serverCommand);

  if (IsKeyPressed(KEY_ENTER))
    _currentScreen = Screen::ENDING;
}

void gui::GameEngine::updateEndingScreen() {
  if (IsKeyPressed(KEY_ENTER))
    _currentScreen = Screen::TITLE;
}

void gui::GameEngine::renderLogoScreen() {
  DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
  DrawText("Fake loading... Please wait 2 seconds.", 230, 220, 20, GRAY);
}

void gui::GameEngine::renderTitleScreen() {
  DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GREEN);
  DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
  DrawText("PRESS ENTER to JUMP to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);
}

void gui::GameEngine::renderGameplayScreen() {
  DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, PURPLE);
  DrawText("GAMEPLAY SCREEN", 20, 20, 40, MAROON);
  DrawText("PRESS ENTER to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);

  DrawText(TextFormat("Map Size: %dx%d", _mapSize.width, _mapSize.height), 20,
           80, 20, WHITE);
  DrawText(TextFormat("Time Unit: %d", _timeUnit.time), 20, 110, 20, WHITE);

  DrawText("Teams:", 20, 140, 20, WHITE);
  for (size_t i = 0; i < _teamNames.names.size() && i < 5; i++)
    DrawText(_teamNames.names[i].c_str(), 80, 170 + (i * 25), 16, WHITE);
}

void gui::GameEngine::renderEndingScreen() {
  DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLUE);
  DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
  DrawText("PRESS ENTER to JUMP to TITLE SCREEN", 130, 220, 20, DARKBLUE);
}

void gui::GameEngine::processServerCommand(const std::string& command) {
  if (command.starts_with("msz")) {
    setMapSize(command);
  } else if (command.starts_with("sgt")) {
    setTimeUnit(command);
  } else if (command.starts_with("tna")) {
    setTeamNames(command);
  }
}
