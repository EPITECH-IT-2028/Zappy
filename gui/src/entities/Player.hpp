#pragma once
#include <array>
#include <string>
#include "Orientation.hpp"

namespace gui {
  /**
   * @brief Represents a player in the Zappy game world.
   *
   * Stores player ID, position, orientation, level, team name, and inventory.
   */
  struct Player {
      /**
       * @brief Number of resource types in the player's inventory.
       */
      static constexpr int RESOURCE_COUNT = 7;
      /**
       * @brief Unique identifier for the player.
       */
      int id;
      /**
       * @brief X coordinate of the player's position on the map.
       */
      int x;
      /**
       * @brief Y coordinate of the player's position on the map.
       */
      int y;
      /**
       * @brief The orientation (direction) the player is facing.
       */
      gui::Orientation orientation;
      /**
       * @brief The current level of the player.
       */
      int level;
      /**
       * @brief The name of the team the player belongs to.
       */
      std::string teamName;
      /**
       * @brief Array holding the count of each resource type in the player's inventory.
       * The index corresponds to the resource type.
       */
      std::array<int, RESOURCE_COUNT> inventoryPlayer;

      /**
       * @brief Constructs a Player with the given attributes.
       *
       * @param id Unique player ID.
       * @param x X coordinate on the map.
       * @param y Y coordinate on the map.
       * @param orientation Player's facing direction.
       * @param level Player's level.
       * @param teamName Name of the player's team.
       */
      Player(int id, int x, int y, Orientation orientation, int level,
             const std::string& teamName) noexcept
          : id(id),
            x(x),
            y(y),
            orientation(orientation),
            level(level),
            teamName(teamName),
            inventoryPlayer{} {
      }
  };
}  // namespace gui
