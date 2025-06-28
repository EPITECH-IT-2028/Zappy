#include "GameEngine.hpp"
#include <raylib.h>
#include <algorithm>
#include <cstddef>
#include <functional>
#include <iostream>
#include <ostream>
#include <unordered_map>

gui::GameEngine::GameEngine(network::ServerCommunication &serverCommunication)
    : _window(SCREEN_WIDTH, SCREEN_HEIGHT, "Zappy"),
      _framesCounter(0),
      _currentScreen(Screen::LOGO),
      _serverCommunication(serverCommunication),
      _gameState(0, 0),
      _commandHandler(_gameState),
      _resourcesLoaded(0) {
  if (!IsWindowReady())
    throw std::runtime_error("Failed to initialize Raylib window");
  initialize();
  _camera.SetPosition({15.0f, 10.0f, 30.0f});
  _camera.SetTarget({0.0f, 0.0f, 0.0f});
  _camera.SetUp({0.0f, 1.0f, 0.0f});
  _camera.SetFovy(45.0f);
  _camera.SetProjection(CAMERA_PERSPECTIVE);
}

gui::GameEngine::~GameEngine() {
  if (_resourcesLoaded == 2) {
    UnloadModel(_brick);
    UnloadModel(_goomba);
  }
}

float gui::GameEngine::getWorldScale() const {
  return worldScale;
}

void gui::GameEngine::setWorldScale(float value) {
  worldScale = std::clamp(value, MIN_SCALE, MAX_SCALE);
}

void gui::GameEngine::initialize() {
  try {
    loadResources();
  } catch (const std::exception &e) {
    std::cerr << "Resource initialization failed: " << e.what() << std::endl;
    _resourcesLoaded = 0;
    _currentScreen = Screen::ERROR;
    _errorMessage = std::string("Resource loading failed: ") + e.what();
  }
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
      case Screen::ERROR:
        renderErrorScreen();
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

  moveCamera();
}

void gui::GameEngine::updateEndingScreen() {
  if (IsKeyPressed(KEY_ENTER))
    _currentScreen = Screen::TITLE;
}

void gui::GameEngine::processNetworkMessages() {
  if (!_serverCommunication.isConnected())
    return;

  static const std::unordered_map<std::string, std::function<void(const std::string&)>> commandHandlers = {
    {"msz", [this](const std::string &msg) { _commandHandler.handleMsz(msg); }},
    {"sgt", [this](const std::string &msg) { _commandHandler.handleSgt(msg); }},
    {"tna", [this](const std::string &msg) { _commandHandler.handleTna(msg); }},
    {"bct", [this](const std::string &msg) { _commandHandler.handleBct(msg); }},
    {"pnw", [this](const std::string &msg) { _commandHandler.handlePnw(msg); }},
    {"ppo", [this](const std::string &msg) { _commandHandler.handlePpo(msg); }},
    {"plv", [this](const std::string &msg) { _commandHandler.handlePlv(msg); }},
    {"pin", [this](const std::string &msg) { _commandHandler.handlePin(msg); }},
    {"enw", [this](const std::string &msg) { _commandHandler.handleEnw(msg); }},
    {"ebo", [this](const std::string &msg) { _commandHandler.handleEbo(msg); }},
    {"edi", [this](const std::string &msg) { _commandHandler.handleEdi(msg); }},
    {"pdi", [this](const std::string &msg) { _commandHandler.handlePdi(msg); }},
    {"pic", [this](const std::string &msg) { _commandHandler.handlePic(msg); }},
    {"pie", [this](const std::string &msg) { _commandHandler.handlePie(msg); }},
    {"pfk", [this](const std::string &msg) { _commandHandler.handlePfk(msg); }},
    {"pdr", [this](const std::string &msg) { _commandHandler.handlePdr(msg); }},
    {"pgt", [this](const std::string &msg) { _commandHandler.handlePgt(msg); }},
    {"pex", [this](const std::string &msg) { _commandHandler.handlePex(msg); }},
    {"pbc", [this](const std::string &msg) { _commandHandler.handlePbc(msg); }},
    {"smg", [this](const std::string &msg) { _commandHandler.handleSmg(msg); }},
    {"suc", [this](const std::string &msg) { _commandHandler.handleSuc(msg); }},
    {"sbp", [this](const std::string &msg) { _commandHandler.handleSbp(msg); }}
  };

  try {
    while (_serverCommunication.hasIncomingData()) {
      std::string message = _serverCommunication.receiveMessage();
      if (message.empty()) {
        std::cerr << "Received empty message from server" << std::endl;
        break;
      }
      if (message.back() == '\n')
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
  } catch (const std::exception &e) {
    std::cerr << "Error processing network messages: " << e.what() << std::endl;
  }
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
  DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GAMEPLAY_BACKGROUND_COLOR);
  drawMap();
  drawBroadcastLog();
  DrawText("GAMEPLAY SCREEN", 20, 20, 40, MAROON);
}

void gui::GameEngine::renderEndingScreen() {
  DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLUE);
  DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
  DrawText("PRESS ENTER to JUMP to TITLE SCREEN", 130, 220, 20, DARKBLUE);
}

void gui::GameEngine::renderErrorScreen() {
  DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RED);
  DrawText("ERROR", 20, 20, 40, BLACK);
  DrawText(_errorMessage.c_str(), 60, 120, 20, BLACK);
}

void gui::GameEngine::loadResources() {
  if (_resourcesLoaded == 2)
    return;

  if (!FileExists(BRICK_MODEL_PATH))
    throw std::runtime_error("Model file not found: " +
                             std::string(BRICK_MODEL_PATH));
  else if (!FileExists(GOOMBA_MODEL_PATH))
    throw std::runtime_error("Model file not found: " +
                             std::string(GOOMBA_MODEL_PATH));

  _brick = LoadModel(BRICK_MODEL_PATH);
  _goomba = LoadModel(GOOMBA_MODEL_PATH);

  if (_brick.meshCount > 0 && _brick.materialCount > 0 &&
      _brick.meshes != nullptr) {
    std::cout << "Brick model loaded successfully." << std::endl;
    _resourcesLoaded += 1;
  } else {
    std::cerr << "Error: Failed to load model: " << BRICK_MODEL_PATH
              << std::endl;
    UnloadModel(_brick);
    throw std::runtime_error(std::string("Failed to load model: ") +
                             BRICK_MODEL_PATH);
  }
  if (_goomba.meshCount > 0 && _goomba.materialCount > 0 &&
      _goomba.meshes != nullptr) {
    std::cout << "Goomba model loaded successfully." << std::endl;
    _resourcesLoaded += 1;
  } else {
    std::cerr << "Error: Failed to load model: " << GOOMBA_MODEL_PATH
              << std::endl;
    UnloadModel(_goomba);
    throw std::runtime_error(std::string("Failed to load model: ") +
                             GOOMBA_MODEL_PATH);
  }
}

void gui::GameEngine::drawMap() {
  if (_resourcesLoaded != 2) {
    std::cerr << "Resources not loaded, cannot draw map." << std::endl;
    return;
  }

  float brickSpacing = BRICK_SPACING * worldScale;
  float mapWidth = static_cast<float>(_gameState.map.width);
  float mapHeight = static_cast<float>(_gameState.map.height);
  Vector3 gridOrigin = {-((mapWidth - 1) * brickSpacing) / 2.0f, 0.0f,
                        -((mapHeight - 1) * brickSpacing) / 2.0f};
  std::vector<std::pair<Vector2, int>> resourceCount;

  BeginMode3D(_camera);
  for (std::size_t y = 0; y < _gameState.map.height; ++y) {
    for (std::size_t x = 0; x < _gameState.map.width; ++x) {
      Vector3 position = {gridOrigin.x + x * brickSpacing, gridOrigin.y,
                          gridOrigin.z + y * brickSpacing};
      DrawModel(_brick, position, BRICK_MODEL_SCALE * worldScale, GRAY);
      Vector3 offset = {WIREFRAME_OFFSET_X * worldScale,
                        WIREFRAME_OFFSET_Y * worldScale,
                        WIREFRAME_OFFSET_Z * worldScale};
      DrawCubeWires(
          {position.x + offset.x, position.y + offset.y, position.z + offset.z},
          BRICK_SPACING * worldScale, BRICK_SPACING * worldScale,
          BRICK_SPACING * worldScale, WHITE);
      drawResource(position, x, y, resourceCount);
    }
  }
  EndMode3D();

  for (const auto &info : resourceCount) {
    Vector2 screenPos = info.first;
    int count = info.second;
    DrawText(TextFormat("%d", count), static_cast<int>(screenPos.x) + 10,
             static_cast<int>(screenPos.y) - 5, 15, WHITE);
  }
}

void gui::GameEngine::drawResource(
    const Vector3 position, int x, int y,
    std::vector<std::pair<Vector2, int>> &resourceTexts) {
  const gui::Tile &tile = _gameState.map.tiles[x][y];

  for (int i = 0; i < static_cast<int>(gui::Tile::RESOURCE_COUNT); i++) {
    int resourceCount = tile.resources[i];
    if (resourceCount > 0) {
      Color color = tile.getResourceColor(static_cast<gui::Tile::Resource>(i));
      Vector3 resourcePosition = {
          position.x + SPHERE_BASE_X * worldScale,
          position.y + SPHERE_BASE_Y * worldScale,
          position.z + SPHERE_BASE_Z * worldScale -
              i * SPHERE_HORIZONTAL_SPACING * worldScale};
      DrawSphere(resourcePosition, 0.035f * worldScale, color);
      Vector2 screenPos = GetWorldToScreen(resourcePosition, _camera);
      resourceTexts.push_back(std::make_pair(screenPos, resourceCount));
    }
  }
}

void gui::GameEngine::moveCamera() {
  handleCameraMovement();
  handleCameraRotation();
  handleCameraZoom();

  if (IsKeyPressed(KEY_R))
    resetCamera();
}

void gui::GameEngine::handleCameraMovement() {
  float moveSpeed = MOVEMENT_BASE_SPEED / worldScale;

  Vector3 forward =
      Vector3Normalize(Vector3Subtract(_camera.target, _camera.position));
  Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, _camera.up));
  Vector3 up = _camera.up;
  Vector3 flatForward = Vector3Normalize({forward.x, 0.0f, forward.z});
  Vector3 flatRight = Vector3Normalize({right.x, 0.0f, right.z});

  if (IsKeyDown(KEY_W)) {
    _camera.position =
        Vector3Add(_camera.position, Vector3Scale(flatForward, moveSpeed));
    _camera.target =
        Vector3Add(_camera.target, Vector3Scale(flatForward, moveSpeed));
  }
  if (IsKeyDown(KEY_A)) {
    _camera.position =
        Vector3Subtract(_camera.position, Vector3Scale(flatRight, moveSpeed));
    _camera.target =
        Vector3Subtract(_camera.target, Vector3Scale(flatRight, moveSpeed));
  }
  if (IsKeyDown(KEY_S)) {
    _camera.position =
        Vector3Subtract(_camera.position, Vector3Scale(flatForward, moveSpeed));
    _camera.target =
        Vector3Subtract(_camera.target, Vector3Scale(flatForward, moveSpeed));
  }
  if (IsKeyDown(KEY_D)) {
    _camera.position =
        Vector3Add(_camera.position, Vector3Scale(flatRight, moveSpeed));
    _camera.target =
        Vector3Add(_camera.target, Vector3Scale(flatRight, moveSpeed));
  }

  if (IsKeyDown(KEY_SPACE)) {
    _camera.position =
        Vector3Add(_camera.position, Vector3Scale(up, moveSpeed));
    _camera.target = Vector3Add(_camera.target, Vector3Scale(up, moveSpeed));
  }
  if (IsKeyDown(KEY_LEFT_SHIFT)) {
    _camera.position =
        Vector3Subtract(_camera.position, Vector3Scale(up, moveSpeed));
    _camera.target =
        Vector3Subtract(_camera.target, Vector3Scale(up, moveSpeed));
  }
}

void gui::GameEngine::handleCameraRotation() {
  Vector3 direction = Vector3Subtract(_camera.target, _camera.position);
  Vector3 right = Vector3Normalize(Vector3CrossProduct(direction, _camera.up));
  Matrix rotation;

  if (IsKeyDown(KEY_UP)) {
    rotation = MatrixRotate(right, ROTATE_SPEED * DEG2RAD);
    direction = Vector3Transform(direction, rotation);
    _camera.target = Vector3Add(_camera.position, direction);
  }
  if (IsKeyDown(KEY_DOWN)) {
    rotation = MatrixRotate(right, -ROTATE_SPEED * DEG2RAD);
    direction = Vector3Transform(direction, rotation);
    _camera.target = Vector3Add(_camera.position, direction);
  }
  if (IsKeyDown(KEY_LEFT)) {
    rotation = MatrixRotateY(ROTATE_SPEED * DEG2RAD);
    direction = Vector3Transform(direction, rotation);
    _camera.target = Vector3Add(_camera.position, direction);
  }
  if (IsKeyDown(KEY_RIGHT)) {
    rotation = MatrixRotateY(-ROTATE_SPEED * DEG2RAD);
    direction = Vector3Transform(direction, rotation);
    _camera.target = Vector3Add(_camera.position, direction);
  }
}

void gui::GameEngine::handleCameraZoom() {
  float wheel = GetMouseWheelMove();

  if (wheel != 0.0f) {
    setWorldScale(worldScale + wheel * SCALE_STEP);
  }
}

void gui::GameEngine::resetCamera() {
  _camera.SetPosition({15.0f, 10.0f, 30.0f});
  _camera.SetTarget({0.0f, 0.0f, 0.0f});
  _camera.SetUp({0.0f, 1.0f, 0.0f});
  _camera.SetFovy(45.0f);
  _camera.SetProjection(CAMERA_PERSPECTIVE);
  setWorldScale(1.0f);
}

void gui::GameEngine::drawBroadcastLog() {
  const int maxVisibleMessages = GameState::MAX_BROADCAST_LOG_SIZE;
  const int startX = BROADCAST_LOG_START_X;
  const int startY = BROADCAST_LOG_START_Y;
  const int lineHeight = BROADCAST_LOG_LINE_HEIGHT;
  const int fontSize = BROADCAST_LOG_FONT_SIZE;

  int messagesToShow = std::min(
      static_cast<int>(_gameState.broadcastLog.size()), maxVisibleMessages);
  int startIndex = std::max(
      0, static_cast<int>(_gameState.broadcastLog.size()) - messagesToShow);

  for (int i = 0; i < messagesToShow; ++i) {
    int messageIndex = startIndex + i;
    int yPos = startY + i * lineHeight;
    DrawText(_gameState.broadcastLog[messageIndex].c_str(), startX, yPos,
             fontSize, DARKGRAY);
  }
}
