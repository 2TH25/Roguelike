#include "Map.h"
#include "Tile.h"

#include <cassert>
#include <optional>
#include <algorithm>
#include <gf/Geometry.h>
#include <gf/TileLayer.h>
#include <gf/Tileset.h>
namespace rCMI {
  gf::Texture& texture = resources.getTexture("../../data/RogueCMI/fond.png");
  gf::Vector2u tileSize = (32,32);
  gf::Tileset tileset(texture,tileSize);

  gf::Vector2i mapSize(10, 10); 
  gf::TileLayer tileLayer(tileset);
  tileLayer.setMapSize(mapSize);

  for (unsigned y = 0; y < mapSize.y; ++y) {
    for (unsigned x = 0; x < mapSize.x; ++x) {
        tileLayer.setTile({x, y}, 1); // tile normale (floor)
        
        if (x == 0 || y == 0) {
            tileLayer.setTile({x, y}, 5); // tile bordure (wall)
        }
    }
  }
  // std::optional<std::size_t> Map::target_actor_at(gf::Vector2i target)
  // {
  //   auto iterator = std::find_if(actors.begin(), actors.end(), [target](const Actor& actor) {
  //     return actor.entity.position == target;
  //   });

  //   if (iterator == actors.end()) {
  //     return std::nullopt;
  //   }

  //   return std::distance(actors.begin(), iterator);
  // }

  // bool Map::blocking_entity_at(gf::Vector2i target)
  // {
  //   auto iterator = std::find_if(actors.begin(), actors.end(), [target](const Actor& actor) {
  //     return actor.entity.blocks_movement && actor.entity.position == target;
  //   });

  //   return iterator != actors.end();
  // }

  // std::vector<gf::Vector2i> Map::compute_path(gf::Vector2i origin, gf::Vector2i target)
  // {
  //   grid.clear_blocks();

  //   for (auto& actor : actors) {
  //     if (actor.entity.blocks_movement) {
  //       grid.set_blocked(actor.entity.position);
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

  //       if (std::any_of(map.actors.begin(), map.actors.end(), [position](const Actor& other) { return position == other.entity.position; })) {
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