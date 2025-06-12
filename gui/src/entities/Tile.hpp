#pragma once
#include <array>
#include <vector>

namespace gui {
  struct Tile{
    enum Resource { Food, Linemate, Deraumere, Sibur, Mendiane, Phiras, Thystame, ResourceCount };
    std::array<int, ResourceCount> resources{};

    std::vector<int> playerIdsOnTile;
    std::vector<int> eggIdsOnTile;   

    bool isEmpty() const {
      return playerIdsOnTile.empty() && eggIdsOnTile.empty();
    }
  };
};