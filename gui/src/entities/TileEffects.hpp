#pragma once

constexpr int TILE_SIZE = 64;
constexpr float BORDER_THICKNESS = 4.0f;

namespace gui {
  /**
   * @brief Manages and renders visual effects for a map tile (e.g., incantations, resource drops).
   *
   * Handles the state and animation of effects such as incantation progress, result highlights,
   * fork, drop, collect, and push events for a tile in the Zappy game world.
   */
  struct TileEffects {
      /**
       * @brief True if an incantation is currently in progress on this tile.
       */
      bool incantationInProgress = false;
      /**
       * @brief True if a result effect (success/failure) should be shown.
       */
      bool showResultEffect = false;
      /**
       * @brief True if the last result effect was a success, false if failure.
       */
      bool resultSuccess = false;
      /**
       * @brief Timer for how long the result effect should be displayed (in seconds).
       */
      float effectTimer = 0.0f;

      /**
       * @brief Starts the incantation visual effect.
       */
      void startIncantationEffect();
      /**
       * @brief Stops the incantation visual effect.
       */
      void stopIncantationEffect();
      /**
       * @brief Shows a success result effect (e.g., green highlight).
       */
      void showSuccessEffect();
      /**
       * @brief Shows a failure result effect (e.g., red highlight).
       */
      void showFailureEffect();
      /**
       * @brief Triggers a result effect (success or failure).
       * @param success True for success, false for failure.
       */
      void resultEffect(bool success);

      /**
       * @brief Shows a fork effect (e.g., egg laid visual).
       */
      void showForkEffect();
      /**
       * @brief Shows a resource drop effect for a given resource.
       * @param resourceId The ID of the resource dropped.
       */
      void showDropEffect(int resourceId);
      /**
       * @brief Shows a resource collect effect for a given resource.
       * @param resourceId The ID of the resource collected.
       */
      void showCollectEffect(int resourceId);
      /**
       * @brief Shows a push effect for a given player.
       * @param playerId The ID of the player pushed.
       */
      void showPushEffect(int playerId);

      /**
       * @brief Updates the state of all effects (timers, animations).
       * @param deltaTime Time elapsed since last update (in seconds).
       */
      void update(float deltaTime);
      /**
       * @brief Draws all active effects for this tile at the given position.
       * @param x The X coordinate (in pixels).
       * @param y The Y coordinate (in pixels).
       */
      void draw(int x, int y);
  };
}  // namespace gui
