#pragma once
#include <string>

namespace gui {
  struct Egg {
    int id;
    int x;
    int y;
    int layingPlayerId;
    std::string teamName;

    Egg(int id, int x, int y, int layingPlayerId, const std::string& teamName)
      : id(id), x(x), y(y), layingPlayerId(layingPlayerId), teamName(teamName) {}
  };
}