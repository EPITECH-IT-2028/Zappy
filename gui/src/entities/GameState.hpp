#pragma once
#include "Map.hpp"
#include "Player.hpp"
#include "Egg.hpp"
#include <cstddef>
#include <unordered_map>

namespace gui {
  struct GameState {
    Map map;
    std::unordered_map<int, Player>players;
    std::unordered_map<int, Egg> eggs;

    GameState(std::size_t width, std::size_t height)
      : map(width, height) {}

    const Player &getPlayerById(int id) const;
    const Egg &getEggById(int id) const;
  };
}