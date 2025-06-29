#pragma once
#include <vector>

namespace gui {
  /**
   * @brief Represents the visual effect of an incantation in progress on the map.
   *
   * Stores the incantation's position, level, involved players, timing, and result state.
   */
  struct IncantationEffect {
      /**
       * @brief X coordinate of the incantation on the map.
       */
      int x;
      /**
       * @brief Y coordinate of the incantation on the map.
       */
      int y;
      /**
       * @brief Level of the incantation.
       */
      int level;
      /**
       * @brief List of player IDs participating in the incantation.
       */
      std::vector<int> players;
      /**
       * @brief Time elapsed since the incantation started (in seconds).
       */
      float timeSinceStart = 0.f;
      /**
       * @brief True if the incantation was successful, false otherwise.
       */
      bool success = false;
      /**
       * @brief True if the incantation effect has finished displaying.
       */
      bool finished = false;

      /**
       * @brief Constructs an IncantationEffect with the given attributes.
       *
       * @param x X coordinate on the map.
       * @param y Y coordinate on the map.
       * @param level Incantation level.
       * @param players List of player IDs involved in the incantation.
       */
      IncantationEffect(int x, int y, int level, std::vector<int> players)
          : x(x), y(y), level(level), players(std::move(players)) {
      }
  };
}  // namespace gui
