#ifndef RL_MAP_H
#define RL_MAP_H

#include <gf/TileLayer.h>
#include <gf/Random.h>

#include "Character.h"

namespace rCMI {
  struct Map {
    std::vector<Character> characters;
    gf::TileLayer map;

    Character& hero() { return characters.front(); }

    std::optional<std::size_t> target_character_at(gf::Vector2i target);
    bool blocking_entity_at(gf::Vector2i target);

    std::vector<gf::Vector2i> compute_path(gf::Vector2i origin, gf::Vector2i target);
  };

  Map generate_dungeon(gf::Vector2i size, int max_rooms, int room_min_size, int room_max_size, int max_monsters_per_room, gf::Random* random);

};

#endif /