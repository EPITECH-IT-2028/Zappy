#include "Map.hpp"
#include <stdexcept>
#include <string>

/**
 * @brief Resizes the map to the specified width and height.
 *
 * @param w New width (number of tiles horizontally).
 * @param h New height (number of tiles vertically).
 * @throws std::invalid_argument if either dimension is zero.
 */
void gui::Map::resize(std::size_t w, std::size_t h) {
  if (w == 0 || h == 0)
    throw std::invalid_argument("Map dimensions must be greater than zero");
  if (w == width && h == height)
    return;

  width = w;
  height = h;
  tiles.resize(height);
  for (auto& row : tiles)
    row.resize(width);
}

/**
 * @brief Returns a reference to the tile at the specified coordinates.
 *
 * @param x X coordinate (column index).
 * @param y Y coordinate (row index).
 * @return Reference to the tile at (x, y).
 * @throws std::out_of_range if (x, y) is not inside the map.
 */
gui::Tile& gui::Map::getTile(std::size_t x, std::size_t y) {
  ensureInBounds(x, y);
  return tiles[y][x];
}

/**
 * @brief Returns a const reference to the tile at the specified coordinates.
 *
 * @param x X coordinate (column index).
 * @param y Y coordinate (row index).
 * @return Const reference to the tile at (x, y).
 * @throws std::out_of_range if (x, y) is not inside the map.
 */
const gui::Tile& gui::Map::getTile(std::size_t x, std::size_t y) const {
  ensureInBounds(x, y);
  return tiles[y][x];
}

/**
 * @brief Checks if the given coordinates are inside the map bounds.
 *
 * @param x X coordinate to check.
 * @param y Y coordinate to check.
 * @return true if (x, y) is inside the map, false otherwise.
 */
bool gui::Map::isInside(std::size_t x, std::size_t y) const {
  return x < width && y < height;
}

/**
 * @brief Throws an exception if the given coordinates are out of map bounds.
 *
 * @param x X coordinate to check.
 * @param y Y coordinate to check.
 * @throws std::out_of_range if (x, y) is not inside the map.
 */
void gui::Map::ensureInBounds(std::size_t x, std::size_t y) const {
  if (x >= width || y >= height) {
    throw std::out_of_range("Tile coordinates out of map (" +
                            std::to_string(x) + ", " + std::to_string(y) + ")");
  }
}
