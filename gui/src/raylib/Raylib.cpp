#include "Raylib.hpp"
#include <functional>
#include <iostream>
#include <ostream>
#include <unordered_map>

gui::Raylib::Raylib(network::ServerCommunication& serverCommunication)
    : _window(SCREEN_WIDTH, SCREEN_HEIGHT, "Zappy"),
      _framesCounter(0),
      _currentScreen(GameScreen::LOGO),
      _serverCommunication(serverCommunication),
      _gameState(0, 0),
      _commandHandler(_gameState) {
  if (!IsWindowReady())
    throw std::runtime_error("Failed to initialize Raylib window");
}

void gui::Raylib::run() {
  _window.SetTargetFPS(60);

  while (!WindowShouldClose()) {
    processNetworkMessages();
    switch (_currentScreen) {
      case GameScreen::LOGO:
        updateLogoScreen();
        break;
      case GameScreen::TITLE:
        updateTitleScreen();
        break;
      case GameScreen::GAMEPLAY:
        updateGameplayScreen();
        break;
      case GameScreen::ENDING:
        updateEndingScreen();
        break;
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

void gui::Raylib::updateLogoScreen() {
  _framesCounter++;
  if (_framesCounter > LOGO_DURATION_FRAMES) {
    _currentScreen = GameScreen::TITLE;
    _framesCounter = 0;
  }
}

void gui::Raylib::updateTitleScreen() {
  if (IsKeyPressed(KEY_ENTER))
    _currentScreen = GameScreen::GAMEPLAY;
}

void gui::Raylib::updateGameplayScreen() {
  if (IsKeyPressed(KEY_ENTER))
    _currentScreen = GameScreen::ENDING;
}

void gui::Raylib::updateEndingScreen() {
  if (IsKeyPressed(KEY_ENTER))
    _currentScreen = GameScreen::TITLE;
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

void gui::Raylib::processNetworkMessages() {
  if (!_serverCommunication.isConnected())
    return;

  static const std::unordered_map<std::string, std::function<void(const std::string&)>> commandHandlers = {
    {"msz", [this](const std::string& msg) { _commandHandler.handleMsz(msg); }},
    {"sgt", [this](const std::string& msg) { _commandHandler.handleSgt(msg); }},
    {"tna", [this](const std::string& msg) { _commandHandler.handleTna(msg); }},
    {"bct", [this](const std::string& msg) { _commandHandler.handleBct(msg); }},
    {"pnw", [this](const std::string& msg) { _commandHandler.handlePnw(msg); }},
    {"ppo", [this](const std::string& msg) { _commandHandler.handlePpo(msg); }},
    {"plv", [this](const std::string& msg) { _commandHandler.handlePlv(msg); }},
    {"pin", [this](const std::string& msg) { _commandHandler.handlePin(msg); }},
    {"enw", [this](const std::string& msg) { _commandHandler.handleEnw(msg); }},
    {"ebo", [this](const std::string& msg) { _commandHandler.handleEbo(msg); }},
    {"edi", [this](const std::string& msg) { _commandHandler.handleEdi(msg); }},
    {"pdi", [this](const std::string& msg) { _commandHandler.handlePdi(msg); }}
  };

  try {
    while (_serverCommunication.hasIncomingData()) {
      std::string message = _serverCommunication.receiveMessage();
      if (message.empty()) {
        std::cerr << "Received empty message from server" << std::endl;
        break;
      }
      if (!message.empty() && message.back() == '\n')
        message.pop_back();

      std::string commandPrefix = message.substr(0, 3);
      auto it = commandHandlers.find(commandPrefix);
      if (it != commandHandlers.end()) {
        it->second(message);
      } else if (message == "WELCOME") {
        std::cout << "Received server welcome message" << std::endl;
      } else {
        std::cout << "Unknown command received: " << message << std::endl;
      }
    }
  } catch (const std::exception& e) {
    std::cerr << "Error processing network messages: " << e.what() << std::endl;
  }
}
