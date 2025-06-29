#include "GameState.hpp"
#include <stdexcept>
#include <string>

/**
 * @brief Gets a const reference to a player by their ID.
 *
 * @param id The player ID.
 * @return Const reference to the Player object.
 * @throws std::runtime_error if the player ID does not exist.
 */
const gui::Player &gui::GameState::getPlayerById(int id) const {
  if (auto it = players.find(id); it != players.end())
    return it->second;
  throw std::runtime_error("Player not found: id=" + std::to_string(id));
}

/**
 * @brief Gets a const reference to an egg by its ID.
 *
 * @param id The egg ID.
 * @return Const reference to the Egg object.
 * @throws std::runtime_error if the egg ID does not exist.
 */
const gui::Egg &gui::GameState::getEggById(int id) const {
  if (auto it = eggs.find(id); it != eggs.end())
    return it->second;
  throw std::runtime_error("Egg not found: id=" + std::to_string(id));
}
