#pragma once
#include <string>

namespace gui {
  struct Player {
    int id;
    std::size_t x;
    std::size_t y;
    int orientation;
    int level;
    std::string teamName;

    Player(int id, std::size_t x, std::size_t y, int orientation, int level, const std::string& teamName)
      : id(id), x(x), y(y), orientation(orientation), level(level), teamName(teamName) {}
  };
};