#include "Tile.hpp"
#include "raylib.h"
#include <iostream>

static constexpr int TILE_SIZE = 64;
static constexpr float BORDER_THICKNESS = 4.0f;

bool gui::Tile::isEmpty() const {
  return playerIdsOnTile.empty() && eggIdsOnTile.empty();
}


void gui::Tile::startIncantationEffect() {
  incantationInProgress = true;
}

void gui::Tile::stopIncantationEffect() {
  incantationInProgress = false;
}

void gui::Tile::resultEffect(bool success) {
  stopIncantationEffect();
  showResultEffect = true;
  resultSuccess = success;
  effectTimer = 2.0f;
}

void gui::Tile::showSuccessEffect() {
  resultEffect(true);
}

void gui::Tile::showFailureEffect() {
  resultEffect(false);
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
    DrawRectangle(x, y, TILE_SIZE, TILE_SIZE, Fade(PURPLE, 0.3f));
  }
  if (showResultEffect) {
    Color color = resultSuccess ? GREEN : RED;
    DrawRectangleLinesEx({(float)x, (float)y, (float)TILE_SIZE, (float)TILE_SIZE}, BORDER_THICKNESS, color);
  }
}

void gui::Tile::showForkEffect() {
  // A placer une sprite de noeunoeuf
}

void gui::Tile::showDropEffect(int resourceId) {
  std::cout << "Drop effect for resource ID: " << resourceId << std::endl;
  // DOit ajouter un effet
}

void gui::Tile::showCollectEffect(int resourceId) {
  std::cout << "Collect effect for resource ID: " << resourceId << std::endl;
  // DOit ajouter un effet
}

void gui::Tile::showPushEffect(int resourceId) {
  std::cout << "Push effect for resource ID: " << resourceId << std::endl;
  // DOit ajouter un effet
}