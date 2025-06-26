#pragma once
#include <cstddef>
#include <unordered_map>
#include "Egg.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "IncantationsEffect.hpp"

namespace gui {
  struct GameState {
      Map map;
      int timeUnit = 0;
      std::vector<std::string> teamNames;
      std::unordered_map<int, Player> players;
      std::unordered_map<int, Egg> eggs;

      std::vector<IncantationEffect> activeIncantations;

      const Player &getPlayerById(int id) const;
      const Egg &getEggById(int id) const;

      GameState(std::size_t width, std::size_t height) : map(width, height) {
      }
  };
}  // namespace gui
