#pragma once
#include <cstddef>
#include <vector>
#include "Tile.hpp"

namespace gui {
  struct Map {
      std::size_t width;
      std::size_t height;
      std::vector<std::vector<Tile>> tiles;

      Tile& getTile(std::size_t x, std::size_t y);
      const Tile& getTile(std::size_t x, std::size_t y) const;
      bool isInside(std::size_t x, std::size_t y) const;

      Map(std::size_t w, std::size_t h)
          : width(w), height(h), tiles(h, std::vector<Tile>(w)) {
      }

    private:
      void ensureInBounds(std::size_t x, std::size_t y) const;
  };
}  // namespace gui
