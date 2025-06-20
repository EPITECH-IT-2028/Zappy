#pragma once
#include <array>
#include <string>
#include "Orientation.hpp"

namespace gui {
  struct Player {
      static constexpr int RESOURCE_COUNT = 7;
      int id;
      int x;
      int y;
      gui::Orientation orientation;
      int level;
      std::string teamName;
      std::array<int, RESOURCE_COUNT> inventoryPlayer;

      Player(int id, int x, int y, Orientation orientation, int level,
             const std::string& teamName) noexcept
          : id(id),
            x(x),
            y(y),
            orientation(orientation),
            level(level),
            teamName(teamName),
            inventoryPlayer{} {
      }
  };
}  // namespace gui
