#include "GameEngine.hpp"
#include <raylib.h>
#include <algorithm>
#include <cstddef>
#include <functional>
#include <iostream>
#include <ostream>
#include <unordered_map>
#include "entities/Map.hpp"
#define RLIGHTS_IMPLEMENTATION
#include <cfloat>
#include "header/rlights.h"

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
  if (!FileExists("resources/egg.png")) {
    std::cerr << "Texture file not found: resources/egg.png" << std::endl;
    throw std::runtime_error("Texture file not found: resources/egg.png");
  }
  _eggTexture = LoadTexture("resources/egg.png");
  if (_eggTexture.id == 0) {
    std::cerr << "Failed to load texture: resources/egg.png" << std::endl;
    throw std::runtime_error("Failed to load texture: resources/egg.png");
  }
}

gui::GameEngine::~GameEngine() {
  if (_resourcesLoaded == TOTAL_MODELS) {
    UnloadModel(_brick);
    UnloadModel(_goomba);
  }
  if (_eggTexture.id != 0)
    UnloadTexture(_eggTexture);
  if (_lightingShader.id != 0)
    UnloadShader(_lightingShader);
  if (_backgroundLogo.id != 0)
    UnloadTexture(_backgroundLogo);
}

float gui::GameEngine::getWorldScale() const {
  return worldScale;
}

void gui::GameEngine::setWorldScale(float value) {
  worldScale = std::clamp(value, MIN_SCALE, MAX_SCALE);
}

void gui::GameEngine::initialize() {
  try {
    loadModels();
    loadShaders();
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
        LogoScreenInput();
        break;
      case Screen::TITLE:
        TitleScreenInput();
        break;
      case Screen::GAMEPLAY:
        GameplayScreenInput();
        break;
      case Screen::ENDING:
        EndingScreenInput();
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

void gui::GameEngine::LogoScreenInput() {
  _framesCounter++;
  if (_framesCounter > LOGO_DURATION_FRAMES) {
    _currentScreen = Screen::TITLE;
    _framesCounter = 0;
  }
}

void gui::GameEngine::TitleScreenInput() {
  if (IsKeyPressed(KEY_ENTER))
    _currentScreen = Screen::GAMEPLAY;
}

void gui::GameEngine::GameplayScreenInput() {
  if (IsKeyPressed(KEY_ENTER))
    _currentScreen = Screen::ENDING;

  moveCamera();
}

void gui::GameEngine::EndingScreenInput() {
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
    {"sbp", [this](const std::string &msg) { _commandHandler.handleSbp(msg); }},
    {"seg", [this](const std::string &msg) { _commandHandler.handleSeg(msg); }},
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

void gui::GameEngine::loadResources() {
  if (!FileExists("resources/MarioBack.png")) {
    std::cerr << "Error: Background logo image not found.\n";
    throw std::runtime_error("Failed to load texture: resources/MarioBack.png");
  }
  _backgroundLogo = LoadTexture("resources/MarioBack.png");
  if (_backgroundLogo.id == 0) {
    std::cerr << "Error: Failed to load logo background image.\n";
    throw std::runtime_error("Failed to load texture: resources/MarioBack.png");
  }
}

void gui::GameEngine::dimensionAsset() {
  if (_backgroundLogo.id == 0) {
    std::cerr << "Warning: Cannot calculate dimensions for invalid texture.\n";
    return;
  }
  _scaleAsset = 2.7f;
  _texWidthAsset = _backgroundLogo.width * _scaleAsset;
  _texHeightAsset = _backgroundLogo.height * _scaleAsset;
  _xAsset = (SCREEN_WIDTH - _texWidthAsset) / 2.0f;
  _yAsset = (SCREEN_HEIGHT - _texHeightAsset) / 2.0f;
}

void gui::GameEngine::renderLogoScreen() {
  if (_backgroundLogo.id != 0) {
    dimensionAsset();
    DrawTextureEx(_backgroundLogo, Vector2{_xAsset, _yAsset}, 0.0f, _scaleAsset,
                  WHITE);
  } else {
    ClearBackground(BLACK);
  }
  _logoText = "ZAPPY";
  _fontSize = 60;
  _textWidth = MeasureText(_logoText.c_str(), _fontSize);
  DrawText(_logoText.c_str(), SCREEN_WIDTH / 2 - _textWidth / 2,
           SCREEN_HEIGHT / 2 - 100, _fontSize, DARKGREEN);

  _dots = (int)(GetTime() * 2) % 4;
  _loadingText = "Loading";
  for (int i = 0; i < _dots; ++i)
    _loadingText += ".";

  _loadingFontSize = 24;
  _loadingWidth = MeasureText(_loadingText.c_str(), _loadingFontSize);
  DrawText(_loadingText.c_str(), SCREEN_WIDTH / 2 - _loadingWidth / 2,
           SCREEN_HEIGHT / 2 + 40, _loadingFontSize, LIGHTGRAY);
}

void gui::GameEngine::renderTitleScreen() {
  if (_backgroundLogo.id != 0) {
    dimensionAsset();
    DrawTextureEx(_backgroundLogo, Vector2{_xAsset, _yAsset}, 0.0f, _scaleAsset,
                  WHITE);
  } else {
    ClearBackground(BLACK);
  }
  _boxWidth = 600;
  _boxHeight = 300;
  _boxX = (SCREEN_WIDTH - _boxWidth) / 2;
  _boxY = (SCREEN_HEIGHT - _boxHeight) / 2;
  DrawRectangleRounded(
      {(float)_boxX, (float)_boxY, (float)_boxWidth, (float)_boxHeight}, 0.2f,
      10, LIGHTGRAY);

  _title = "ZAPPY GAME";
  _titleFontSize = 50;
  _titleTextWidth = MeasureText(_title.c_str(), _titleFontSize);
  DrawText(_title.c_str(), SCREEN_WIDTH / 2 - _titleTextWidth / 2, _boxY + 40,
           _titleFontSize, DARKGREEN);

  _subtitle = "Press ENTER to start";
  _subtitleFontSize = 24;
  _subtitleTextWidth = MeasureText(_subtitle.c_str(), _subtitleFontSize);
  DrawText(_subtitle.c_str(), SCREEN_WIDTH / 2 - _subtitleTextWidth / 2,
           _boxY + _boxHeight - 70, _subtitleFontSize, DARKGRAY);
}

void gui::GameEngine::renderGameplayScreen() {
  updateTileSelection();
  std::vector<std::pair<Vector3, int>> resourceCount;

  DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GAMEPLAY_BACKGROUND_COLOR);
  updateShaders();

  BeginMode3D(_camera);
  BeginShaderMode(_lightingShader);

  drawMap(&resourceCount);
  drawPlayers();
  for (const auto &info : resourceCount) {
    Vector3 worldPos = info.first;
    DrawText3D(GetFontDefault(), TextFormat("%d", info.second),
               {worldPos.x + 0.1f, worldPos.y - 0.05f, worldPos.z - 0.1f},
               0.2f * worldScale, 0.05f, 0.0f, false,
               (Color){255, 255, 255, 255});
  }

  EndShaderMode();
  drawPlayerShadows();
  drawEggShadows();
  drawEggs();
  drawLights();
  EndMode3D();

  drawBroadcastLog();
  drawTileInfoPanel();
  // DrawText("GAMEPLAY SCREEN", 20, 20, 40, MAROON);
}

void gui::GameEngine::renderEndingScreen() {
  std::string msg = "GAME OVER - Team " + _gameState.winningTeamName + " wins!";
  int textWidth = MeasureText(msg.c_str(), 40);

  DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLUE);
  DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
  DrawText("PRESS ENTER to JUMP to TITLE SCREEN", 130, 220, 20, DARKBLUE);
  DrawText(msg.c_str(), (SCREEN_WIDTH - textWidth) / 2, SCREEN_HEIGHT / 2, 40,
           RED);
}

void gui::GameEngine::renderErrorScreen() {
  DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RED);
  DrawText("ERROR", 20, 20, 40, BLACK);
  DrawText(_errorMessage.c_str(), 60, 120, 20, BLACK);
}

void gui::GameEngine::updateShaders() {
  float cameraPos[3] = {_camera.GetPosition().x, _camera.GetPosition().y,
                        _camera.GetPosition().z};
  SetShaderValue(_lightingShader, _lightingShader.locs[SHADER_LOC_VECTOR_VIEW],
                 cameraPos, SHADER_UNIFORM_VEC3);
  for (int i = 0; i < MAX_LIGHTS; ++i)
    UpdateLightValues(_lightingShader, _lights[i]);
}

void gui::GameEngine::loadModels() {
  if (_resourcesLoaded == TOTAL_MODELS)
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

void gui::GameEngine::loadShaders() {
  _lightingShader = LoadShader(TextFormat("resources/lighting.vs", 330),
                               TextFormat("resources/lighting.fs", 330));
  _lightingShader.locs[SHADER_LOC_VECTOR_VIEW] =
      GetShaderLocation(_lightingShader, "viewPos");
  _ambientLoc = GetShaderLocation(_lightingShader, "ambient");
  float ambient[4] = {0.1f, 0.1f, 0.1f, 1.0f};
  SetShaderValue(_lightingShader, _ambientLoc, ambient, SHADER_UNIFORM_VEC4);

  for (int i = 0; i < _brick.materialCount; i++)
    _brick.materials[i].shader = _lightingShader;
  for (int i = 0; i < _goomba.materialCount; i++)
    _goomba.materials[i].shader = _lightingShader;

  _lights[0] =
      CreateLight(LIGHT_DIRECTIONAL, (Vector3){-6, 4, -6}, Vector3Zero(),
                  (Color){255, 255, 255, 255}, _lightingShader);
  _lights[1] = CreateLight(LIGHT_DIRECTIONAL, (Vector3){6, 0, 6}, Vector3Zero(),
                           (Color){32, 32, 32, 255}, _lightingShader);
}

void gui::GameEngine::drawMap(
    std::vector<std::pair<Vector3, int>> *resourceCount) {
  if (_resourcesLoaded != TOTAL_MODELS) {
    std::cerr << "Resources not loaded, cannot draw map." << std::endl;
    return;
  }

  float brickSpacing = BRICK_SPACING * worldScale;
  float mapWidth = static_cast<float>(_gameState.map.width);
  float mapHeight = static_cast<float>(_gameState.map.height);
  Vector3 gridOrigin = {-((mapWidth - 1) * brickSpacing) / 2.0f, 0.0f,
                        -((mapHeight - 1) * brickSpacing) / 2.0f};

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
      drawResource(position, x, y, *resourceCount);
    }
  }
}

void gui::GameEngine::drawResource(
    const Vector3 position, int x, int y,
    std::vector<std::pair<Vector3, int>> &resourceTexts) {
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
      resourceTexts.push_back(std::make_pair(resourcePosition, resourceCount));
    }
  }
}

void gui::GameEngine::drawPlayers() {
  float brickSpacing = BRICK_SPACING * worldScale;
  Vector3 gridOrigin = {
      -((static_cast<float>(_gameState.map.width) - 1) * brickSpacing) / 2.0f,
      0.0f,
      -((static_cast<float>(_gameState.map.height) - 1) * brickSpacing) / 2.0f};

  for (const auto &playerPair : _gameState.players) {
    const gui::Player &player = playerPair.second;
    Vector3 position = {gridOrigin.x + player.x * brickSpacing,
                        gridOrigin.y + 1.1f * worldScale,
                        gridOrigin.z + player.y * brickSpacing};
    float angle = 0.0f;
    switch (player.orientation) {
      case gui::Orientation::NORTH:
        angle = PI;
        break;
      case gui::Orientation::EAST:
        angle = PI / 2.0f;
        break;
      case gui::Orientation::SOUTH:
        angle = 0.0f;
        break;
      case gui::Orientation::WEST:
        angle = 3 * PI / 2.0f;
        break;
      default:
        break;
    }
    DrawModelEx(
        _goomba, position, (Vector3){0, 1, 0}, angle * RAD2DEG,
        (Vector3){0.15f * worldScale, 0.15f * worldScale, 0.15f * worldScale},
        WHITE);
  }
}

Vector3 gui::GameEngine::calculateGridOrigin(float mapWidth, float mapHeight,
                                             float brickSpacing) const {
  return {-(mapWidth - 1) * brickSpacing / 2.0f, 0.0f,
          -(mapHeight - 1) * brickSpacing / 2.0f};
}

void gui::GameEngine::drawEggs() {
  float brickSpacing = BRICK_SPACING * worldScale;
  float mapWidth = static_cast<float>(_gameState.map.width);
  float mapHeight = static_cast<float>(_gameState.map.height);
  Vector3 gridOrigin = calculateGridOrigin(mapWidth, mapHeight, brickSpacing);
  for (const auto &eggPair : _gameState.eggs) {
    const gui::Egg &egg = eggPair.second;
    Vector3 pos = calculateEggPosition(egg, gridOrigin, brickSpacing);
    DrawBillboard(_camera, _eggTexture, pos, 0.5f * worldScale, WHITE);
  }
}

Vector3 gui::GameEngine::calculatePlayerPosition(const gui::Player &player,
                                                 const Vector3 &gridOrigin,
                                                 float brickSpacing) const {
  return {gridOrigin.x + player.x * brickSpacing,
          gridOrigin.y + 1.1f * worldScale,
          gridOrigin.z + player.y * brickSpacing};
}

void gui::GameEngine::drawPlayerShadows() {
  float brickSpacing = BRICK_SPACING * worldScale;
  Vector3 gridOrigin = {
      -((static_cast<float>(_gameState.map.width) - 1) * brickSpacing) / 2.0f,
      0.0f,
      -((static_cast<float>(_gameState.map.height) - 1) * brickSpacing) / 2.0f};

  for (const auto &playerPair : _gameState.players) {
    const gui::Player &player = playerPair.second;
    Vector3 position =
        calculatePlayerPosition(player, gridOrigin, brickSpacing);
    Vector3 shadowPos = {position.x, gridOrigin.y + 1.1f, position.z};
    DrawCylinder(shadowPos, PLAYER_SHADOW_RADIUS * worldScale,
                 PLAYER_SHADOW_RADIUS * worldScale, 0.01f, 32,
                 (Color){0, 0, 0, PLAYER_SHADOW_ALPHA});
  }
}

Vector3 gui::GameEngine::calculateEggPosition(const gui::Egg &egg,
                                              const Vector3 &gridOrigin,
                                              float brickSpacing) const {
  return {gridOrigin.x + egg.x * brickSpacing, gridOrigin.y + 1.4f * worldScale,
          gridOrigin.z + egg.y * brickSpacing};
}

void gui::GameEngine::drawEggShadows() {
  float brickSpacing = BRICK_SPACING * worldScale;
  Vector3 gridOrigin = {-((_gameState.map.width - 1) * brickSpacing) / 2.0f,
                        0.0f,
                        -((_gameState.map.height - 1) * brickSpacing) / 2.0f};
  for (const auto &eggPair : _gameState.eggs) {
    const gui::Egg &egg = eggPair.second;
    Vector3 pos = calculateEggPosition(egg, gridOrigin, brickSpacing);
    Vector3 shadowPos = {pos.x, gridOrigin.y + 1.1f, pos.z};
    DrawCylinder(shadowPos, EGG_SHADOW_RADIUS * worldScale,
                 EGG_SHADOW_RADIUS * worldScale, 0.01f, 32,
                 (Color){0, 0, 0, EGG_SHADOW_ALPHA});
  }
}

void gui::GameEngine::drawLights() {
  for (int i = 0; i < MAX_LIGHTS; ++i) {
    if (_lights[i].enabled) {
      DrawSphereEx(_lights[i].position, 0.2f, 8, 8, _lights[i].color);
    } else {
      DrawSphereWires(_lights[i].position, 0.2f, 8, 8,
                      ColorAlpha(_lights[i].color, 0.3f));
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

void gui::GameEngine::updateTileSelection() {
  float brickSpacing = BRICK_SPACING * worldScale;
  float mapWidth = static_cast<float>(_gameState.map.width);
  float mapHeight = static_cast<float>(_gameState.map.height);
  Vector3 gridOrigin = calculateGridOrigin(mapWidth, mapHeight, brickSpacing);
  int tileX;
  int tileY;

  hoveredTile.valid = false;
  if (getTileUnderMouse(mapWidth, mapHeight, brickSpacing, gridOrigin, tileX,
                        tileY)) {
    hoveredTile.x = tileX;
    hoveredTile.y = tileY;
    hoveredTile.valid = true;
  }
}

void gui::GameEngine::drawTileInfoPanel() {
  const int panelWidth = 260;
  const int panelHeight = 210;
  const int panelX = 20;
  const int panelY = SCREEN_HEIGHT - panelHeight - 20;
  int textY = panelY + 18;
  int textX = panelX + 16;
  int fontSize = 20;
  TileSelection *activeTile = nullptr;

  DrawRectangle(panelX, panelY, panelWidth, panelHeight, Fade(LIGHTGRAY, 1.1f));
  if (hoveredTile.valid)
    activeTile = &hoveredTile;
  if (activeTile && activeTile->valid && activeTile->x >= 0 &&
      activeTile->y >= 0 &&
      activeTile->x < static_cast<int>(_gameState.map.width) &&
      activeTile->y < static_cast<int>(_gameState.map.height)) {
    DrawText(TextFormat("Tile: (%d, %d)", activeTile->x, activeTile->y), textX,
             textY, fontSize, DARKGREEN);
    textY += fontSize + 6;
    const gui::Tile &tile = _gameState.map.tiles[activeTile->x][activeTile->y];
    for (int i = 0; i < static_cast<int>(gui::Tile::RESOURCE_COUNT); ++i) {
      int resourcesCount = tile.resources[i];
      Color resourceColor =
          tile.getResourceColor(static_cast<gui::Tile::Resource>(i));
      DrawRectangle(textX, textY + 2, 16, 16, resourceColor);
      DrawRectangleLines(textX, textY + 2, 16, 16, DARKGRAY);
      std::string resourceName =
          tile.getResourceName(static_cast<gui::Tile::Resource>(i));
      DrawText(TextFormat("%s: %d", resourceName.c_str(), resourcesCount),
               textX + 24, textY, fontSize - 2, BLACK);
      textY += fontSize + 2;
    }
  } else {
    DrawText("Hover a tile to see info", textX, textY, fontSize - 2, GRAY);
  }
}
