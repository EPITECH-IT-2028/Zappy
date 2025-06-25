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
  };
} // namespace gui
