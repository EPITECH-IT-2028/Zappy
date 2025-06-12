#pragma once
#include <cstdint>
#include <string>
#include <cstddef>

namespace gui {
  struct Player {
    int id;
    std::size_t x;
    std::size_t y;
    enum orientation {
      NORTH = 0,
      EAST = 1,
      SOUTH = 2,
      WEST = 3
    };
    orientation orient;
    std::uint8_t level;
    std::string teamName;

    Player(int id, std::size_t x, std::size_t y, int orientation, int level, const std::string& teamName)
      : id(id), x(x), y(y), orient(orient), level(level), teamName(teamName) {}
  };
};