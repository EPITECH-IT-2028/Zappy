#pragma once
#include <cstddef>
#include <vector>
#include "Tile.hpp"

namespace gui {
  /**
   * @brief Represents the game map, consisting of a 2D grid of tiles.
   *
   * Stores the map's width, height, and the collection of tiles. Provides methods for resizing,
   * accessing, and validating tile positions.
   */
  struct Map {
      /**
       * @brief Width of the map (number of tiles horizontally).
       */
      std::size_t width;
      /**
       * @brief Height of the map (number of tiles vertically).
       */
      std::size_t height;
      /**
       * @brief 2D vector containing all tiles in the map.
       * Indexed as tiles[y][x].
       */
      std::vector<std::vector<Tile>> tiles;

      /**
       * @brief Resizes the map to the specified width and height.
       *
       * @param w New width (number of tiles horizontally).
       * @param h New height (number of tiles vertically).
       */
      void resize(std::size_t w, std::size_t h);
      /**
       * @brief Returns a reference to the tile at the specified coordinates.
       *
       * @param x X coordinate (column index).
       * @param y Y coordinate (row index).
       * @return Reference to the tile at (x, y).
       */
      Tile& getTile(std::size_t x, std::size_t y);
      /**
       * @brief Returns a const reference to the tile at the specified coordinates.
       *
       * @param x X coordinate (column index).
       * @param y Y coordinate (row index).
       * @return Const reference to the tile at (x, y).
       */
      const Tile& getTile(std::size_t x, std::size_t y) const;
      /**
       * @brief Checks if the given coordinates are inside the map bounds.
       *
       * @param x X coordinate to check.
       * @param y Y coordinate to check.
       * @return true if (x, y) is inside the map, false otherwise.
       */
      bool isInside(std::size_t x, std::size_t y) const;

      /**
       * @brief Constructs a Map with the given width and height.
       *
       * @param w Width of the map (number of tiles horizontally).
       * @param h Height of the map (number of tiles vertically).
       */
      Map(std::size_t w, std::size_t h)
          : width(w), height(h), tiles(h, std::vector<Tile>(w)) {
      }

    private:
      /**
       * @brief Throws an exception if the given coordinates are out of map bounds.
       *
       * @param x X coordinate to check.
       * @param y Y coordinate to check.
       * @throws std::out_of_range if (x, y) is not inside the map.
       */
      void ensureInBounds(std::size_t x, std::size_t y) const;
  };
}  // namespace gui
