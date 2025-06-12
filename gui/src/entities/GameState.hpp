#pragma once
#include <vector>
#include "Map.hpp"
#include "Player.hpp"
#include "Egg.hpp"

namespace gui {
  struct GameState {
    gui::Map map;
    std::vector<Player> players;
    std::vector<Egg> eggs;

    GameState(std::size_t width, std::size_t height)
      : map(width, height) {}

    Player &getPlayerById(int id) {
    for (auto& player : players)
      if (player.id == id)
        return player;
      throw std::runtime_error("Player not found");
    }

    Egg &getEggById(int id) {
      for (auto& egg : eggs)
        if (egg.id == id)
          return egg;
      throw std::runtime_error("Egg not found");
    }
  };
}