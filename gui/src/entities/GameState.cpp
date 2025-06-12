#include "Player.hpp"
#include "Egg.hpp"
#include "GameState.hpp"
#include <stdexcept>

const gui::Player &gui::GameState::getPlayerById(int id) const {
for (const auto& player : players)
  if (player.id == id)
    return player;
  throw std::runtime_error("Player not found");
}

const gui::Egg &gui::GameState::getEggById(int id) const {
  for (const auto& egg : eggs)
    if (egg.id == id)
      return egg;
  throw std::runtime_error("Egg not found");
}
