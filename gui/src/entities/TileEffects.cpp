#include "TileEffects.hpp"
#include <cmath>
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
  effectTimer = 3.0f;
}

void gui::TileEffects::showSuccessEffect() {
  resultEffect(true);
}

void gui::TileEffects::showFailureEffect() {
  resultEffect(false);
}

void gui::TileEffects::update(float deltaTime) {
  for (auto &inc : _incantationEffects) {
    if (inc.finished) continue;

    inc.timeSinceStart += deltaTime;
    if (inc.timeSinceStart > 3.0f) {
        inc.finished = true;
    }
}
}

void gui::TileEffects::draw3D(int x, int y) {
  if (incantationInProgress) {
    float time = GetTime();
    float height = 1.0f + 0.2f * sinf(time * 4.0f);
    float radius = 0.4f + 0.05f * sinf(time * 3.0f);

    Color effectColor = Fade(PURPLE, 0.6f);
    Vector3 position = { (float)x + 0.5f, 0.0f, (float)y + 0.5f };

    DrawCylinder(position, radius, radius, height, 16, effectColor);
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

void gui::TileEffects::showPushEffect(int playerId) {
  std::cout << "Push effect for player ID: " << playerId << std::endl;
  // TODO: Add push effect
}
