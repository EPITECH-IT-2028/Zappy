#pragma once
#include <string>

namespace gui {
  /**
   * @brief Represents an egg entity in the Zappy game world.
   *
   * Stores the egg's ID, position, laying player, and team name.
   */
  struct Egg {
      /**
       * @brief Unique identifier for the egg.
       */
      int id;
      /**
       * @brief X coordinate of the egg's position on the map.
       */
      int x;
      /**
       * @brief Y coordinate of the egg's position on the map.
       */
      int y;
      /**
       * @brief ID of the player who laid the egg.
       */
      int layingPlayerId;
      /**
       * @brief Name of the team to which the egg belongs.
       */
      std::string teamName;

      /**
       * @brief Constructs an Egg with the given attributes.
       *
       * @param id Unique egg ID.
       * @param x X coordinate on the map.
       * @param y Y coordinate on the map.
       * @param layingPlayerId ID of the player who laid the egg.
       * @param teamName Name of the team the egg belongs to.
       */
      Egg(int id, int x, int y, int layingPlayerId, const std::string& teamName)
          : id(id),
            x(x),
            y(y),
            layingPlayerId(layingPlayerId),
            teamName(teamName) {
      }
  };
}  // namespace gui
