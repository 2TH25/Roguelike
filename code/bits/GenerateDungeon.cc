#include "Map.h"

namespace rCMI {

  Map generate_dungeon(gf::Vector2i size, int max_rooms, int room_min_size, int room_max_size, int max_monsters_per_room, gf::Random* random, gf::Texture& texture) {
    
    Map map;
    map.size = size;
    map.tileLayer = gf::TileLayer::createOrthogonal(size, {32, 32});
    map.tilesetId = map.tileLayer.createTilesetId();
    
    gf::Tileset& tileset = map.tileLayer.getTileset(map.tilesetId);
    tileset.setTexture(texture);
    tileset.setTileSize({32, 32});

    map.grid.resize(size.x * size.y, TileType::Wall);

    return map;
  }
}