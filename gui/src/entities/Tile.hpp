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

      bool isEmpty() const;
  };
};  // namespace gui
