#pragma once
#include <vector>
#include "Map.hpp"
#include "Player.hpp"
#include "Egg.hpp"

namespace gui {
  struct GameState {
    Map map;
    std::vector<Player> players;
    std::vector<Egg> eggs;

    GameState(int width, int height)
      : map(width, height) {}

    const Player &getPlayerById(int id) const;
    const Egg &getEggById(int id) const;
  };
}