#pragma once
#include <raylib.h>
#include <array>
#include <vector>
#include "TileEffects.hpp"

namespace gui {
  struct Tile {
      static constexpr int RESOURCE_COUNT = 7;

      enum class Resource {
        FOOD,
        LINEMATE,
        DERAUMERE,
        SIBUR,
        MENDIANE,
        PHIRAS,
        THYSTAME
      };

      std::array<int, RESOURCE_COUNT> resources{};
      std::vector<int> playerIdsOnTile;
      std::vector<int> eggIdsOnTile;
      TileEffects effects;

      bool isEmpty() const;
      Color getResourceColor(Resource type) const;
      std::string getResourceName(Resource type) const;
  };
}  // namespace gui
