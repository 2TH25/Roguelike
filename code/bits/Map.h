#ifndef MAP_H
#define MAP_H

#include <gf/TileLayer.h>
#include <gf/Texture.h>
#include <vector>
#include <gf/Random.h>
#include <optional>

#include "Tile.h"
#include "Character.h"

namespace rCMI {
  class Map {
  public:

    Character& hero() { return characters.front(); }

    std::optional<std::size_t> target_character_at(gf::Vector2i target);
    bool blocking_entity_at(gf::Vector2i target);

    std::vector<gf::Vector2i> compute_path(gf::Vector2i origin, gf::Vector2i target);
    void update_tile_at(gf::Vector2i pos, TileType type);

    bool isVisible(gf::Vector2i pos) const { 
    return true; 
  }

    gf::Vector2i getSize() const { return size; }
    std::vector<Character>& getCharacters() { return characters; }
    gf::TileLayer& getTileLayer() { return tileLayer; }
    std::size_t getTilesetId() const { return tilesetId; }
    void setSize(gf::Vector2i newSize) { size = newSize; }
    void setCharacters(const std::vector<Character>& chars) { characters = chars; }
    void setTilesetId(std::size_t id) { tilesetId = id;}

  private: 
    gf::Vector2i size;
    std::vector<TileType> grid;
    std::vector<Character> characters;
    gf::TileLayer tileLayer;
    std::size_t tilesetId;

  };

  Map generate_dungeon(gf::Vector2i size, int max_rooms, int room_min_size, int room_max_size, int max_monsters_per_room, gf::Random* random, gf::Texture& texture);
}

#endif