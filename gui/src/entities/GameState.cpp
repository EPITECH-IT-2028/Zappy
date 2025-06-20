#include "GameState.hpp"
#include <stdexcept>
#include <string>

const gui::Player &gui::GameState::getPlayerById(int id) const {
  if (auto it = players.find(id); it != players.end())
    return it->second;
  throw std::runtime_error("Player not found: id=" + std::to_string(id));
}

const gui::Egg &gui::GameState::getEggById(int id) const {
  if (auto it = eggs.find(id); it != eggs.end())
    return it->second;
  throw std::runtime_error("Egg not found: id=" + std::to_string(id));
}
