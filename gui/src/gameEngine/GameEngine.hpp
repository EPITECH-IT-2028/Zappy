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
  /**
   * @brief Enum representing the different screens in the game.
   */
  enum class Screen {
    LOGO,      ///< Logo/loading screen
    TITLE,     ///< Title/main menu screen
    GAMEPLAY,  ///< Main gameplay screen
    ENDING,    ///< Ending/score screen
    ERROR      ///< Error screen
  };

  /**
   * @brief Structure representing the selection state of a tile.
   */
  struct TileSelection {
      int x = -1;      ///< X coordinate of the selected tile
      int y = -1;      ///< Y coordinate of the selected tile
      bool valid = false; ///< Whether the selection is valid
  };

  /**
   * @class GameEngine
   * @brief Main class responsible for running and rendering the game GUI.
   */
  class GameEngine {
    public:
      /**
       * @brief Constructs the GameEngine.
       * @param serverCommunication Reference to the server communication handler.
       */
      GameEngine(network::ServerCommunication &serverCommunication);
      /**
       * @brief Destructor for GameEngine.
       */
      ~GameEngine();

      /**
       * @brief Gets the current world scale.
       * @return The world scale factor.
       */
      float getWorldScale() const;
      /**
       * @brief Sets the world scale.
       * @param value The new world scale factor.
       */
      void setWorldScale(float value);

      /**
       * @brief Main loop to run the game engine.
       */
      void run();
      /**
       * @brief Initializes the game engine and resources.
       */
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

      static constexpr int PLAYER_PANEL_WIDTH = 260;
      static constexpr int PLAYER_PANEL_HEIGHT = 320;
      static constexpr int PLAYER_PANEL_X = SCREEN_WIDTH - PLAYER_PANEL_WIDTH - 20;
      static constexpr int PLAYER_PANEL_Y = 20;

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
      TileSelection clickedTile;
      int selectedPlayerId = -1;

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

      Shader _lightingShader; ///< Shader used for lighting effects.
      Light _lights[2]; ///< Array of lights in the scene.
      int _ambientLoc; ///< Shader location for ambient light.
      Texture2D _eggTexture; ///< Texture for rendering eggs.

      // Title Screen
      int _boxWidth; ///< Width of the title box.
      int _boxHeight; ///< Height of the title box.
      int _boxX; ///< X position of the title box.
      int _boxY; ///< Y position of the title box.

      std::string _title; ///< Main title text.
      int _titleFontSize; ///< Font size for the title.
      int _titleTextWidth; ///< Width of the title text.

      std::string _subtitle; ///< Subtitle text.
      int _subtitleFontSize; ///< Font size for the subtitle.
      int _subtitleTextWidth; ///< Width of the subtitle text.

      // Wait screen
      std::string _logoText; ///< Text displayed on the logo/wait screen.
      int _fontSize; ///< Font size for logo/wait screen text.
      int _textWidth; ///< Width of the logo/wait screen text.

      int _dots; ///< Number of dots in the loading animation.
      std::string _loadingText; ///< Loading text.

      int _loadingFontSize; ///< Font size for loading text.
      int _loadingWidth; ///< Width of the loading text.

      Texture _backgroundLogo; ///< Texture for the background logo.
      /**
       * @brief Loads all required resources (textures, models, etc.).
       */
      void loadResources();

      /**
       * @brief Calculates and sets the dimensions for an asset.
       */
      void dimensionAsset();
      float _scaleAsset; ///< Scale factor for the asset.
      float _texWidthAsset; ///< Texture width of the asset.
      float _texHeightAsset; ///< Texture height of the asset.
      float _xAsset; ///< X position of the asset.
      float _yAsset; ///< Y position of the asset.

      /**
       * @brief Updates the currently selected tile based on mouse position.
       */
      void updateTileSelection();
      /**
       * @brief Draws the information panel for the currently selected tile.
       */
      void drawTileInfoPanel();
      /**
       * @brief Gets the tile under the mouse cursor.
       * @param mapWidth Width of the map.
       * @param mapHeight Height of the map.
       * @param brickSpacing Spacing between bricks.
       * @param gridOrigin Origin of the grid in world coordinates.
       * @param tileX Output: X coordinate of the tile under the mouse.
       * @param tileY Output: Y coordinate of the tile under the mouse.
       * @return true if a tile is under the mouse, false otherwise.
       */
      bool getTileUnderMouse(float mapWidth, float mapHeight,
                             float brickSpacing, Vector3 gridOrigin, int &tileX,
                             int &tileY);
      /**
       * @brief Draws the panel listing all players.
       */
      void drawPlayerListPanel();
      /**
       * @brief Draws the information panel for the selected player.
       */
      void drawPlayerInfoPanel();
  };
}  // namespace gui
