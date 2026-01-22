#ifndef MAP_H
#define MAP_H

#include <gf/Tileset.h>
#include <gf/Grid.h>
#include <gf/Map.h>
#include <gf/Texture.h>
#include <vector>
#include <gf/Random.h>
#include <optional>

#include "Tile.h"
#include "Character.h"

namespace rCMI
{
  struct posWhBool
  {
    static posWhBool getPosWhBool(gf::Vector2i pos, bool active)
    {
      posWhBool res;
      res.position = pos;
      res.isActive = active;
      return res;
    }

    gf::Vector2i position;
    bool isActive;
  };

  class RogueCMI;

  class Map
  {
  public:
    Map(RogueCMI *game);

    void render(gf::RenderTarget &target, const gf::RenderStates &states);

    void update_tile_at(gf::Vector2i pos, TileType type);

    void generate_board();

    bool blocking_entity_at(gf::Vector2i target);

    bool isWalkable(gf::Vector2i position) const;

    gf::Vector2i getSize() { return size; };

    bool isStairs(gf::Vector2i position) const;

    bool isInFieldOfVision(gf::Vector2i position) { return map.isInFieldOfVision(position); };

    void generate_dungeon(gf::Vector2i Map_size);

    void clearFieldOfVision() { map.clearFieldOfVision(); };

    void computeFieldOfVision(gf::Vector2i pos) { map.computeFieldOfVision(pos, 8); };

    bool isHealing(gf::Vector2i position) const;

    bool usHealing(gf::Vector2i position);

  private:
    RogueCMI *m_game = nullptr;
    gf::Vector2i size;
    gf::SquareMap map;
    gf::Vector2i stairs;
    std::vector<posWhBool> healingZone;
  };
};

#endif