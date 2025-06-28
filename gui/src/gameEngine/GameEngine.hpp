#pragma once

#include <raylib.h>
#include "commandHandler/CommandHandler.hpp"
#include "entities/GameState.hpp"
#include "header/rlights.h"
#include "raylib-cpp.hpp"
#include "server/ServerCommunication.hpp"

#define LOGO_DURATION_FRAMES 120
#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

#define CAMERA_ZOOM_STEP 0.5f
#define MIN_CAMERA_FOVY 6.0f
#define MAX_CAMERA_FOVY 45.0f

#define BRICK_MODEL_PATH "resources/mario_brick/scene.gltf"
#define GOOMBA_MODEL_PATH "resources/goomba.glb"

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
    ENDING,
    ERROR
  };

  class GameEngine {
    public:
      GameEngine(network::ServerCommunication &serverCommunication);
      ~GameEngine();

      float getWorldScale() const;
      void setWorldScale(float value);

      void run();
      void initialize();

    private:
      static constexpr float SPHERE_HORIZONTAL_SPACING = 0.15f;
      static constexpr float SPHERE_BASE_X = -0.4f;
      static constexpr float SPHERE_BASE_Y = 1.15f;
      static constexpr float SPHERE_BASE_Z = 0.6f;
      static constexpr Color GAMEPLAY_BACKGROUND_COLOR = {11, 94, 158, 255};

      static constexpr float MOVEMENT_BASE_SPEED = 0.2f;
      static constexpr float ROTATE_SPEED = 0.7f;
      static constexpr float MIN_SCALE = 0.2f;
      static constexpr float MAX_SCALE = 5.0f;
      static constexpr float SCALE_STEP = 0.1f;

      static constexpr int BROADCAST_LOG_START_X = 10;
      static constexpr int BROADCAST_LOG_START_Y = 10;
      static constexpr int BROADCAST_LOG_LINE_HEIGHT = 25;
      static constexpr int BROADCAST_LOG_FONT_SIZE = 20;

      void DrawTextCodepoint3D(Font font, int codepoint, Vector3 position,
                               float fontSize, bool backface, Color tint);
      void DrawText3D(Font font, const char *text, Vector3 position,
                      float fontSize, float fontSpacing, float lineSpacing,
                      bool backface, Color tint);

      raylib::Window _window;
      int _framesCounter;
      raylib::Camera3D _camera;

      Screen _currentScreen;
      network::ServerCommunication &_serverCommunication;
      GameState _gameState;
      handlecommand::CommandHandler _commandHandler;
      float worldScale = 1.0f;

      void LogoScreenInput();
      void TitleScreenInput();
      void GameplayScreenInput();
      void EndingScreenInput();
      void processNetworkMessages();

      void renderLogoScreen();
      void renderTitleScreen();
      void renderGameplayScreen();
      void renderEndingScreen();
      void renderErrorScreen();

      void updateShaders();

      void loadModels();
      void loadShaders();
      Model _brick;
      Model _goomba;
      std::size_t _resourcesLoaded;
      std::string _errorMessage;

      void drawMap(std::vector<std::pair<Vector3, int>> *resourceCount);
      void drawResource(const Vector3 position, int x, int y,
                        std::vector<std::pair<Vector3, int>> &resourceTexts);
      void drawPlayers();
      void drawLights();
      void moveCamera();
      void handleCameraMovement();
      void handleCameraRotation();
      void handleCameraZoom();
      void resetCamera();
      void drawBroadcastLog();

      Shader _lightingShader;
      Light _lights[2];
      int _ambientLoc;
  };
}  // namespace gui
