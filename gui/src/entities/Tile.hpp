#pragma once
#include <array>
#include <vector>

namespace gui {
  struct Tile {
    static constexpr int RESOURCE_COUNT = 7;

    enum class Resource {
      Food,
      Linemate,
      Deraumere,
      Sibur,
      Mendiane,
      Phiras,
      Thystame
    };

    std::array<int, RESOURCE_COUNT> resources{};
    std::vector<int> playerIdsOnTile;
    std::vector<int> eggIdsOnTile;

    bool incantationInProgress = false;
    bool showResultEffect = false;
    bool resultSuccess = false;
    float effectTimer = 0.0f;

    bool isEmpty() const;

    void startIncantationEffect();
    void stopIncantationEffect();
    void showSuccessEffect();
    void showFailureEffect();

    void update(float deltaTime);
    void draw(int x, int y);
  };
}

