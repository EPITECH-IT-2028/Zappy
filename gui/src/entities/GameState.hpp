#pragma once
#include <cstddef>
#include <unordered_map>
#include "Egg.hpp"
#include "IncantationsEffect.hpp"
#include "Map.hpp"
#include "Player.hpp"

namespace gui {
  /**
   * @brief Represents the complete state of the Zappy game world.
   *
   * Stores the map, players, eggs, incantations, teams, broadcast log, and game status.
   */
  struct GameState {
      /**
       * @brief The game map (2D grid of tiles).
       */
      Map map;
      /**
       * @brief The current time unit (tick rate) of the game.
       */
      int timeUnit = 0;
      /**
       * @brief List of all team names in the game.
       */
      std::vector<std::string> teamNames;
      /**
       * @brief Map of player IDs to Player objects.
       */
      std::unordered_map<int, Player> players;
      /**
       * @brief Map of egg IDs to Egg objects.
       */
      std::unordered_map<int, Egg> eggs;
      /**
       * @brief True if the game is over, false otherwise.
       */
      bool isGameOver = false;
      /**
       * @brief Name of the winning team (if the game is over).
       */
      std::string winningTeamName = "";

      /**
       * @brief List of currently active incantation effects.
       */
      std::vector<IncantationEffect> activeIncantations;

      /**
       * @brief Gets a const reference to a player by their ID.
       *
       * @param id The player ID.
       * @return Const reference to the Player object.
       * @throws std::out_of_range if the player ID does not exist.
       */
      const Player &getPlayerById(int id) const;
      /**
       * @brief Gets a const reference to an egg by its ID.
       *
       * @param id The egg ID.
       * @return Const reference to the Egg object.
       * @throws std::out_of_range if the egg ID does not exist.
       */
      const Egg &getEggById(int id) const;

      /**
       * @brief Log of recent broadcast messages (up to MAX_BROADCAST_LOG_SIZE).
       */
      std::vector<std::string> broadcastLog;
      /**
       * @brief Maximum number of broadcast messages to keep in the log.
       */
      static constexpr std::size_t MAX_BROADCAST_LOG_SIZE = 10;
      /**
       * @brief Constructs a GameState with a map of the given size.
       *
       * @param width Width of the map (number of tiles horizontally).
       * @param height Height of the map (number of tiles vertically).
       */
      GameState(std::size_t width, std::size_t height) : map(width, height) {
      }
  };
}  // namespace gui
