#include "TileEffects.hpp"
#include <iostream>
#include "raylib.h"

void gui::TileEffects::startIncantationEffect() {
  incantationInProgress = true;
}

void gui::TileEffects::stopIncantationEffect() {
  incantationInProgress = false;
}

void gui::TileEffects::resultEffect(bool success) {
  if (!incantationInProgress) {
    std::cerr
        << "Warning: Result effect triggered without active incantation\n";
  }
  showResultEffect = true;
  resultSuccess = success;
  effectTimer = 2.0f;
}

void gui::TileEffects::showSuccessEffect() {
  resultEffect(true);
}

void gui::TileEffects::showFailureEffect() {
  resultEffect(false);
}

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

void gui::TileEffects::showForkEffect() {
  // TODO: Add fork effect (e.g., show an egg sprite)
}

void gui::TileEffects::showDropEffect(int resourceId) {
  std::cout << "Drop effect for resource ID: " << resourceId << std::endl;
  // TODO: Add drop effect
}

void gui::TileEffects::showCollectEffect(int resourceId) {
  std::cout << "Collect effect for resource ID: " << resourceId << std::endl;
  // TODO: Add collect effect
}
