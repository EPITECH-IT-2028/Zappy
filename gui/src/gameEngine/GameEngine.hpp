#pragma once

#include <raylib.h>
#include "commandHandler/CommandHandler.hpp"
#include "entities/GameState.hpp"
#include "raylib-cpp.hpp"
#include "server/ServerCommunication.hpp"

#define LOGO_DURATION_FRAMES 120
#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

#define CAMERA_ZOOM_STEP 0.5f
#define MIN_CAMERA_FOVY 6.0f
#define MAX_CAMERA_FOVY 45.0f

#define BRICK_MODEL_PATH "resources/mario_brick/scene.gltf"

#define BRICK_SPACING 1.1f
#define BRICK_MODEL_SCALE 0.01f
#define WIREFRAME_OFFSET_X 0.0f
#define WIREFRAME_OFFSET_Y 0.55f
#define WIREFRAME_OFFSET_Z 0.15f

namespace gui {
  enum class Screen {
    LOGO,
    TITLE,
    GAMEPLAY,
    ENDING
  };

  class GameEngine {
    public:
      GameEngine(network::ServerCommunication &serverCommunication);
      ~GameEngine() = default;

      void run();
      void initialize();

    private:
      raylib::Window _window;
      int _framesCounter;
      raylib::Camera3D _camera;

      Screen _currentScreen;
      network::ServerCommunication &_serverCommunication;
      GameState _gameState;
      handlecommand::CommandHandler _commandHandler;

      void updateLogoScreen();
      void updateTitleScreen();
      void updateGameplayScreen();
      void updateEndingScreen();
      void processNetworkMessages();

      void renderLogoScreen();
      void renderTitleScreen();
      void renderGameplayScreen();
      void renderEndingScreen();

      void loadResources();
      raylib::Model _brick;
      bool _resourcesLoaded;

      void drawMap();
  };
}  // namespace gui
