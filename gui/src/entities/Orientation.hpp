#pragma once

namespace gui {
  /**
   * @brief Enum representing the possible orientations (directions) a player can face.
   */
  enum class Orientation {
    NORTH = 1, ///< Facing north (up)
    EAST = 2,  ///< Facing east (right)
    SOUTH = 3, ///< Facing south (down)
    WEST = 4   ///< Facing west (left)
  };
}  // namespace gui
