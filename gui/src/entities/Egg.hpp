#pragma once
#include <string>
#include <cstddef>

namespace gui {
  struct Egg {
    int id;
    int x;
    int y;
    int layingPlayerId;
    std::string teamName;

    Egg(int id, std::size_t x, std::size_t y, int layingPlayerId, const std::string& teamName)
      : id(id), x(x), y(y), layingPlayerId(layingPlayerId), teamName(teamName) {}
  };
}