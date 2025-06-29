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

  struct TileSelection {
      int x = -1;
      int y = -1;
      bool valid = false;
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
      static constexpr std::size_t TOTAL_MODELS = 2;

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

      static constexpr float PLAYER_SHADOW_RADIUS = 0.44f;
      static constexpr int PLAYER_SHADOW_ALPHA = 60;
      static constexpr float EGG_SHADOW_RADIUS = 0.22f;
      static constexpr int EGG_SHADOW_ALPHA = 120;

      static constexpr int TILE_PANEL_WIDTH = 260;
      static constexpr int TILE_PANEL_HEIGHT = 210;
      static constexpr int TILE_PANEL_X = 20;
      static constexpr int TILE_PANEL_Y = SCREEN_HEIGHT - TILE_PANEL_HEIGHT - 20;

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
      TileSelection hoveredTile;

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
      void drawEggs();
      void drawPlayerShadows();
      void drawEggShadows();
      void drawLights();
      void moveCamera();
      void handleCameraMovement();
      void handleCameraRotation();
      void handleCameraZoom();
      void resetCamera();
      void drawBroadcastLog();

      Vector3 calculatePlayerPosition(const gui::Player &player,
                                      const Vector3 &gridOrigin,
                                      float brickSpacing) const;
      Vector3 calculateEggPosition(const gui::Egg &egg,
                                   const Vector3 &gridOrigin,
                                   float brickSpacing) const;
      Vector3 calculateGridOrigin(float mapWidth, float mapHeight,
                                  float brickSpacing) const;

      Shader _lightingShader;
      Light _lights[2];
      int _ambientLoc;
      Texture2D _eggTexture;

      // Title Screen
      int _boxWidth;
      int _boxHeight;
      int _boxX;
      int _boxY;

      std::string _title;
      int _titleFontSize;
      int _titleTextWidth;

      std::string _subtitle;
      int _subtitleFontSize;
      int _subtitleTextWidth;

      // Wait screen
      std::string _logoText;
      int _fontSize;
      int _textWidth;

      int _dots;
      std::string _loadingText;

      int _loadingFontSize;
      int _loadingWidth;

      Texture _backgroundLogo;
      void loadResources();

      void dimensionAsset();
      float _scaleAsset;
      float _texWidthAsset;
      float _texHeightAsset;
      float _xAsset;
      float _yAsset;

      void updateTileSelection();
      void drawTileInfoPanel();
      bool getTileUnderMouse(float mapWidth, float mapHeight,
                             float brickSpacing, Vector3 gridOrigin, int &tileX,
                             int &tileY);
  };
}  // namespace gui
