#pragma once
#include <raylib.h>
#include <array>
#include <string>
#include <vector>
#include "TileEffects.hpp"

namespace gui {
  /**
   * @brief Represents a map tile in the Zappy game world.
   *
   * Stores resource counts, player and egg IDs present on the tile, and visual effects.
   */
  struct Tile {
      /**
       * @brief Number of resource types per tile.
       */
      static constexpr int RESOURCE_COUNT = 7;

      /**
       * @brief Enum representing the different resource types available on a tile.
       */
      enum class Resource {
        FOOD,      ///< Food resource
        LINEMATE,  ///< Linemate resource
        DERAUMERE, ///< Deraumere resource
        SIBUR,     ///< Sibur resource
        MENDIANE,  ///< Mendiane resource
        PHIRAS,    ///< Phiras resource
        THYSTAME   ///< Thystame resource
      };

      /**
       * @brief Array holding the count of each resource type on the tile.
       * The index corresponds to the Resource enum.
       */
      std::array<int, RESOURCE_COUNT> resources{};
      /**
       * @brief List of player IDs currently present on the tile.
       */
      std::vector<int> playerIdsOnTile;
      /**
       * @brief List of egg IDs currently present on the tile.
       */
      std::vector<int> eggIdsOnTile;
      /**
       * @brief Visual effects currently active on the tile.
       */
      TileEffects effects;

      /**
       * @brief Checks if the tile is empty (no resources, players, or eggs).
       * @return true if the tile is empty, false otherwise.
       */
      bool isEmpty() const;
      /**
       * @brief Gets the color associated with a given resource type.
       * @param type The resource type.
       * @return The color for the specified resource.
       */
      Color getResourceColor(Resource type) const;
      /**
       * @brief Gets the display name of a given resource type.
       * @param type The resource type.
       * @return The name of the resource as a string.
       */
      std::string getResourceName(Resource type) const;
  };
}  // namespace gui
