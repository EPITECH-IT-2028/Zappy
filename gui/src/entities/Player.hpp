#pragma once
#include <cstdint>
#include <string>
#include <cstddef>

namespace gui {
  struct Player {
    int id;
    std::size_t x;
    std::size_t y;
    enum Orientation {
      NORTH = 1,
      EAST = 2,
      SOUTH = 3,
      WEST = 4
    };
    Orientation orient;
    std::uint8_t level;
    std::string teamName;

    Player(int id, std::size_t x, std::size_t y, Orientation orientation,
      std::uint8_t level, const std::string& teamName) noexcept
      : id(id), x(x), y(y), orient(orientation), level(level), teamName(teamName) {}
  };
};