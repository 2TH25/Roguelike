#ifndef MAP_H
#define MAP_H

#include <gf/Grid.h>
#include <gf/Random.h>
#include "Tile.h"

namespace rCMI{
  struct Map {
    gf::Grid grid;
    std::vector<gf::Vector2i> compute_path(gf::Vector2i origin, gf::Vector2i target);
  };

  Map generate_dungeon(gf::Vector2i size, int max_rooms, int room_min_size, int room_max_size, int max_monsters_per_room, gf::Random* random);

}

#endif