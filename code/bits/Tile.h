// Dans Tile.h
#ifndef TILE_H
#define TILE_H

#include <gf/Vector.h>
#include <gf/Sprite.h>
#include <gf/RenderTarget.h>

namespace rCMI
{
  enum class TileType : uint32_t
  {
    Floor = 0,
    Wall = 1,
    Stairs = 2,
    HealingFloor = 3
  };

  class Tile
  {
    TileType getType() const { return type; }
    bool isWalkable() const { return walkable; }

    void setType(TileType t)
    {
      type = t;
      walkable = (t != TileType::Wall);
    };

  private:
    TileType type;
    bool walkable;
  };
};
#endif // TILE_H