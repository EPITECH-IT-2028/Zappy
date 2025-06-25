#include "GameEngine.hpp"
#include <functional>
#include <iostream>
#include <ostream>
#include <unordered_map>

gui::GameEngine::GameEngine(network::ServerCommunication& serverCommunication)
    : _window(SCREEN_WIDTH, SCREEN_HEIGHT, "Zappy"),
      _framesCounter(0),
      _currentScreen(Screen::LOGO),
      _serverCommunication(serverCommunication),
      _gameState(0, 0),
      _commandHandler(_gameState),
      _resourcesLoaded(false) {
  if (!IsWindowReady())
    throw std::runtime_error("Failed to initialize Raylib window");
  loadResources();
}

void gui::GameEngine::run() {
  _window.SetTargetFPS(60);

  while (!WindowShouldClose()) {
    processNetworkMessages();
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
  if (IsKeyPressed(KEY_ENTER))
    _currentScreen = Screen::GAMEPLAY;
}

void gui::GameEngine::updateGameplayScreen() {
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
}

void gui::GameEngine::renderEndingScreen() {
  DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLUE);
  DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
  DrawText("PRESS ENTER to JUMP to TITLE SCREEN", 130, 220, 20, DARKBLUE);
}

void gui::GameEngine::processNetworkMessages() {
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

void gui::GameEngine::loadResources() {
  if (_resourcesLoaded)
    return;

  try {
    if (!FileExists("resources/mario_brick/scene.gltf"))
      throw std::runtime_error(
          "Model file not found: resources/mario_brick/scene.gltf");

    _brick = LoadModel("resources/mario_brick/scene.gltf");

    if (_brick.IsValid()) {
      std::cout << "Brick model loaded successfully." << std::endl;
      _resourcesLoaded = true;
    } else {
      std::cerr
          << "Error: Failed to load model: resources/mario_brick/scene.gltf"
          << std::endl;
    }
  } catch (const std::exception& e) {
    std::cerr << "Exception while loading model: " << e.what() << std::endl;
  }
}
