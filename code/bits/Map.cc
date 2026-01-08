#include "Map.h"
#include "Tile.h"

#include <cassert>
#include <optional>
#include <algorithm>
#include <gf/Geometry.h>
#include <gf/TileLayer.h>
#include <gf/Tileset.h>


namespace rCMI {

  Map::Map(RogueCMI *game)
  : gf::Entity(0)
  , m_game(game)
  {}


  std::optional<std::size_t> Map::target_character_at(gf::Vector2i target) {
      for (std::size_t i = 0; i < characters.size(); ++i) {
          if (characters[i].getExistence().getPosition() == target) return i;
      }
      return std::nullopt;
  }

  bool Map::blocking_entity_at(gf::Vector2i target) {
      if (grid[target.y * size.x + target.x] == TileType::Wall) {
        return true;
      }

      return target_character_at(target).has_value();
  }

  void Map::update_tile_at(gf::Vector2i pos, TileType type) {
      grid[pos.y * size.x + pos.x] = type;
      int tileIndex = (type == TileType::Wall) ? 1 : 0;
      tileLayer.setTile(pos, tilesetId, tileIndex);
  }

  bool Map::isWalkable(gf::Vector2i position) const {

    if (position.x < 0 || position.y < 0 || position.x >= size.x || position.y >= size.y) {
        return false;
    }
    std::size_t index = position.y * size.x + position.x;
    return tiles[index].isWalkable();
}

  void Map::render(gf::RenderTarget& renderer) {
    for (auto& tile : getTiles()) {
      tile.render(renderer);
    }
  }

  Map Map::generate_board(gf::Vector2i size) {

    gf::TileMap tileMap(size);
    gf::Texture tilesetTexture(m_game->resources.getTexture("tileSetTexture.jpg"));
    gf::Tileset tileset(tilesetTexture, {80, 80});

    for(int y = 0; y < size.y; ++y) {
      for(int x = 0; x size.x; ++x) {
        if(x ==0 || y == 0 || x == size.x - 1 || y == size.y - 1) {
          tileMap.setTile({x,y}, Wall);
        } else {
          tileMap.setTile({x,y}, Floor);
        }
      }
    }

    gf::TileLayer layer(tileset, tileMap);
}











  // std::optional<std::size_t> Map::target_actor_at(gf::Vector2i target)
  // {
  //   auto iterator = std::find_if(actors.begin(), actors.end(), [target](const Actor& actor) {
  //     return actor.existence.position == target;
  //   });

  //   if (iterator == actors.end()) {
  //     return std::nullopt;
  //   }

  //   return std::distance(actors.begin(), iterator);
  // }

  // bool Map::blocking_entity_at(gf::Vector2i target)
  // {
  //   auto iterator = std::find_if(actors.begin(), actors.end(), [target](const Actor& actor) {
  //     return actor.existence.blocks_movement && actor.existence.position == target;
  //   });

  //   return iterator != actors.end();
  // }

  // std::vector<gf::Vector2i> Map::compute_path(gf::Vector2i origin, gf::Vector2i target)
  // {
  //   grid.clear_blocks();

  //   for (auto& actor : actors) {
  //     if (actor.existence.blocks_movement) {
  //       grid.set_blocked(actor.existence .position);
  //     }
  //   }

  //   const gf::RouteCost cost = { 1.0f, gf::Sqrt2, 5.0f };
  //   return grid.compute_route(origin, target, cost);
  // }



  // /*
  //  * Procedural Generation
  //  */

  // namespace {
  //   void dig_straight_tunnel_between(gf::Grid& grid, gf::Vector2i start, gf::Vector2i end)
  //   {
  //     assert(start.x == end.x || start.y == end.y);

  //     gf::BresenhamAlgorithm bresenham(start, end);

  //     while (auto maybe_current = bresenham.step()) {
  //       auto current = *maybe_current;
  //       grid.set_empty(current);
  //       grid.set_tag(current, Tile::Floor);
  //     }
  //   }

  //   void dig_tunnel_between(gf::Grid& grid, gf::Vector2i start, gf::Vector2i end, gf::Random* random)
  //   {
  //     gf::Vector2i corner = { 0, 0 };

  //     if (random->compute_bernoulli(0.5)) {
  //       // Move horizontally, then vertically.
  //       corner.x = end.x;
  //       corner.y = start.y;
  //     } else {
  //       // Move vertically, then horizontally.
  //       corner.x = start.x;
  //       corner.y = end.y;
  //     }

  //     dig_straight_tunnel_between(grid, start, corner);
  //     dig_straight_tunnel_between(grid, corner, end);
  //   }

  //   void place_objects(gf::RectI room, Map& map, int max_monsters_per_room, gf::Random* random)
  //   {
  //     int monsters_count = random->compute_uniform_integer(0, max_monsters_per_room);

  //     for (int i = 0; i < monsters_count; ++i) {

  //       auto position = random->compute_position(room.shrink_by(1));

  //       if (std::any_of(map.actors.begin(), map.actors.end(), [position](const Actor& other) { return position == other.existence.position; })) {
  //         continue;
  //       }

  //       if (random->compute_bernoulli(0.8)) {
  //         // orc
  //         map.actors.push_back(Actor::orc(position));
  //       } else {
  //         // troll
  //         map.actors.push_back(Actor::troll(position));
  //       }

  //     }
  //   }

  // }

  // Map generate_dungeon(gf::Vector2i size, int max_rooms, int room_min_size, int room_max_size, int max_monsters_per_room, gf::Random* random)
  // {
  //   Map map;

  //   map.grid = gf::Grid::make_orthogonal(size);
  //   map.grid.reset(gf::None);

  //   std::vector<gf::RectI> rooms;

  //   for (int i = 0; i < max_rooms; ++i) {
  //     gf::Vector2i room_size = { 0, 0 };
  //     room_size.w = random->compute_uniform_integer(room_min_size, room_max_size);
  //     room_size.h = random->compute_uniform_integer(room_min_size, room_max_size);

  //     gf::Vector2i room_position = { 0, 0 };
  //     room_position.x = random->compute_uniform_integer(0, size.w - room_size.w);
  //     room_position.y = random->compute_uniform_integer(0, size.h - room_size.h);

  //     const gf::RectI room = gf::RectI::from_position_size(room_position, room_size);

  //     if (std::any_of(rooms.begin(), rooms.end(), [room](const gf::RectI& other_room) { return room.intersects(other_room); })) {
  //       continue;
  //     }

  //     for (auto position : gf::rectangle_range(room.shrink_by(1))) {
  //       map.grid.set_empty(position);
  //       map.grid.set_tag(position, Tile::Floor);
  //     }

  //     if (rooms.empty()) {
  //       map.actors.push_back(Actor::hero(room.center()));
  //     } else {
  //       dig_tunnel_between(map.grid, rooms.back().center(), room.center(), random);
  //     }

  //     place_objects(room, map, max_monsters_per_room, random);

  //     rooms.push_back(room);
  //   }

  //   return map;
 // }

}