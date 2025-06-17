#pragma once
#include <cstdint>
#include <string>

namespace gui {
  struct Player {
    int id;
    int x;
    int y;
    enum Orientation {
      NORTH = 1,
      EAST = 2,
      SOUTH = 3,
      WEST = 4
    };
    Orientation orientation;
    std::uint8_t level;
    std::string teamName;

    Player(int id, int x, int y, Orientation orientation,
      std::uint8_t level, const std::string& teamName) noexcept
      : id(id), x(x), y(y), orientation(orientation), level(level), teamName(teamName) {}
  };
}