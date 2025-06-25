#include "Tile.hpp"
#include "raylib.h"

bool gui::Tile::isEmpty() const {
  return playerIdsOnTile.empty() && eggIdsOnTile.empty();
}


void gui::Tile::startIncantationEffect() {
  incantationInProgress = true;
}

void gui::Tile::stopIncantationEffect() {
  incantationInProgress = false;
}

void gui::Tile::showSuccessEffect() {
  stopIncantationEffect();
  showResultEffect = true;
  resultSuccess = true;
  effectTimer = 2.0f;
}

void gui::Tile::showFailureEffect() {
  stopIncantationEffect();
  showResultEffect = true;
  resultSuccess = false;
  effectTimer = 2.0f;
}

void gui::Tile::update(float deltaTime) {
  if (showResultEffect) {
    effectTimer -= deltaTime;
    if (effectTimer <= 0.0f) {
      showResultEffect = false;
    }
  }
}

void gui::Tile::draw(int x, int y) {
  if (incantationInProgress) {
    DrawRectangle(x, y, 64, 64, Fade(PURPLE, 0.3f));
  }
  if (showResultEffect) {
    Color color = resultSuccess ? GREEN : RED;
    DrawRectangleLinesEx({(float)x, (float)y, 64.0f, 64.0f}, 4.0f, color);
  }
}
