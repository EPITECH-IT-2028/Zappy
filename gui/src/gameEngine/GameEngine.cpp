#include "GameEngine.hpp"
#include <raylib.h>
#include <cstddef>
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
  _camera.SetPosition({5.0f, 5.0f, 10.0f});
  _camera.SetTarget({0.0f, 0.0f, 0.0f});
  _camera.SetUp({0.0f, 1.0f, 0.0f});
  _camera.SetFovy(45.0f);
  _camera.SetProjection(CAMERA_ORTHOGRAPHIC);
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
  if (_camera.projection == CAMERA_ORTHOGRAPHIC && GetMouseWheelMove() > 0 &&
      _camera.fovy > 6.0f) {
    _camera.fovy -= 0.5f;
    std::cout << "Camera fovy: " << _camera.fovy << std::endl;
  } else if (_camera.projection == CAMERA_ORTHOGRAPHIC &&
             GetMouseWheelMove() < 0 && _camera.fovy < 45.0f) {
    _camera.fovy += 0.5f;
    std::cout << "Camera fovy: " << _camera.fovy << std::endl;
  }
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
  DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, LIGHTGRAY);
  drawMap();
  DrawText("GAMEPLAY SCREEN", 20, 20, 40, MAROON);
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

void gui::GameEngine::drawMap() {
  if (!_resourcesLoaded) {
    std::cerr << "Resources not loaded, cannot draw map." << std::endl;
    return;
  }

  float brickSpacing = 1.1f;
  float mapWidth = static_cast<float>(_gameState.map.width);
  float mapHeight = static_cast<float>(_gameState.map.height);
  Vector3 gridOrigin = {-((mapWidth - 1) * brickSpacing) / 2.0f, 0.0f,
                        -((mapHeight - 1) * brickSpacing) / 2.0f};

  BeginMode3D(_camera);
  for (std::size_t y = 0; y < _gameState.map.height; ++y) {
    for (std::size_t x = 0; x < _gameState.map.width; ++x) {
      Vector3 position = {gridOrigin.x + x * brickSpacing, gridOrigin.y,
                          gridOrigin.z + y * brickSpacing};
      DrawModel(_brick, position, 0.01f, GRAY);
      Vector3 offset = {0.0f, 0.55f, 0.15f};
      DrawCubeWires(
          {position.x + offset.x, position.y + offset.y, position.z + offset.z},
          1.1f, 1.1f, 1.1f, WHITE);
    }
  }
  EndMode3D();
}
