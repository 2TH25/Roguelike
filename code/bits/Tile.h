#ifndef TILE_H
#define TILE_H

#include <gf/Vector.h>

namespace rCMI {  

  enum class TileType {
    Floor,
    Wall
  };

  struct Tile {
      TileType type;
      bool walkable;
      int textureIndex;
  };
};
#endif