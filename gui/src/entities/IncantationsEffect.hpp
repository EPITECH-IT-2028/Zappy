#pragma once
#include <vector>

namespace gui {
  struct IncantationEffect {
      int x;
      int y;
      int level;
      std::vector<int> players;
      float timeSinceStart = 0.f;
      bool success = false;
      bool finished = false;

      IncantationEffect(int x, int y, int level, std::vector<int> players)
          : x(x), y(y), level(level), players(std::move(players)) {
      }
  };
}  // namespace gui
