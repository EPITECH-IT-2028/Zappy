#include "TileEffects.hpp"
#include <iostream>
#include "raylib.h"

/**
 * @brief Starts the incantation visual effect for this tile.
 */
void gui::TileEffects::startIncantationEffect() {
  incantationInProgress = true;
}

/**
 * @brief Stops the incantation visual effect for this tile.
 */
void gui::TileEffects::stopIncantationEffect() {
  incantationInProgress = false;
}

/**
 * @brief Triggers a result effect (success or failure) after an incantation.
 *
 * @param success True for a success effect, false for a failure effect.
 */
void gui::TileEffects::resultEffect(bool success) {
  if (!incantationInProgress) {
    std::cerr
        << "Warning: Result effect triggered without active incantation\n";
  }
  showResultEffect = true;
  resultSuccess = success;
  effectTimer = 3.0f;
}

/**
 * @brief Shows a success result effect (e.g., green highlight).
 */
void gui::TileEffects::showSuccessEffect() {
  resultEffect(true);
}

/**
 * @brief Shows a failure result effect (e.g., red highlight).
 */
void gui::TileEffects::showFailureEffect() {
  resultEffect(false);
}

/**
 * @brief Updates the state of all effects (timers, animations).
 *
 * @param deltaTime Time elapsed since last update (in seconds).
 */
void gui::TileEffects::update(float deltaTime) {
  if (deltaTime < 0.0f) {
    std::cerr << "Warning: Negative deltaTime in TileEffects update\n";
    return;
  }
  if (showResultEffect) {
    effectTimer -= deltaTime;
    if (effectTimer <= 0.0f) {
      showResultEffect = false;
    }
  }
}

/**
 * @brief Draws all active effects for this tile at the given position.
 *
 * @param x The X coordinate (in pixels).
 * @param y The Y coordinate (in pixels).
 */
void gui::TileEffects::draw(int x, int y) {
  if (incantationInProgress) {
    DrawRectangle(x, y, TILE_SIZE, TILE_SIZE, Fade(PURPLE, 0.3f));
  }
  if (showResultEffect) {
    Color color = resultSuccess ? GREEN : RED;
    DrawRectangleLinesEx(
        {(float)x, (float)y, (float)TILE_SIZE, (float)TILE_SIZE},
        BORDER_THICKNESS, color);
  }
}

/**
 * @brief Shows a fork effect (e.g., egg laid visual). (Not yet implemented)
 */
void gui::TileEffects::showForkEffect() {
  // TODO: Add fork effect (e.g., show an egg sprite)
}

/**
 * @brief Shows a resource drop effect for a given resource. (Not yet implemented)
 *
 * @param resourceId The ID of the resource dropped.
 */
void gui::TileEffects::showDropEffect(int resourceId) {
  std::cout << "Drop effect for resource ID: " << resourceId << std::endl;
  // TODO: Add drop effect
}

/**
 * @brief Shows a resource collect effect for a given resource. (Not yet implemented)
 *
 * @param resourceId The ID of the resource collected.
 */
void gui::TileEffects::showCollectEffect(int resourceId) {
  std::cout << "Collect effect for resource ID: " << resourceId << std::endl;
  // TODO: Add collect effect
}

/**
 * @brief Shows a push effect for a given player. (Not yet implemented)
 *
 * @param playerId The ID of the player pushed.
 */
void gui::TileEffects::showPushEffect(int playerId) {
  std::cout << "Push effect for player ID: " << playerId << std::endl;
  // TODO: Add push effect
}
