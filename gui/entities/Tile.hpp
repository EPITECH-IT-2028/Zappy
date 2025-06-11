#pragma once
#include <array>
#include <vector>

namespace gui {

  struct Tile{
    std::array<int, 7> resources = {0, 0, 0, 0, 0, 0, 0};

    std::vector<int> playerIdsOnTile;
    std::vector<int> eggIdsOnTile;   

    bool isEmpty() const {
      return playerIdsOnTile.empty() && eggIdsOnTile.empty();
    }
  };
};