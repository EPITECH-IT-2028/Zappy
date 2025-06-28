#pragma once
#include <cstddef>
#include <unordered_map>
#include "Egg.hpp"
#include "IncantationsEffect.hpp"
#include "Map.hpp"
#include "Player.hpp"

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

      std::vector<std::string> broadcastLog;
      static constexpr std::size_t MAX_BROADCAST_LOG_SIZE = 10;

      GameState(std::size_t width, std::size_t height) : map(width, height) {
      }
  };
}  // namespace gui
