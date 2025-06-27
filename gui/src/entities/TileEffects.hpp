#pragma once

constexpr int TILE_SIZE = 64;
constexpr float BORDER_THICKNESS = 4.0f;

namespace gui {
  struct TileEffects {
      bool incantationInProgress = false;
      bool showResultEffect = false;
      bool resultSuccess = false;
      float effectTimer = 0.0f;

      void startIncantationEffect();
      void stopIncantationEffect();
      void showSuccessEffect();
      void showFailureEffect();
      void resultEffect(bool success);

      void showForkEffect();
      void showDropEffect(int resourceId);
      void showCollectEffect(int resourceId);
      void showPushEffect(int playerId);

      void update(float deltaTime);
      void draw(int x, int y);
  };
}  // namespace gui
