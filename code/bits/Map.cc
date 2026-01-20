#include "Map.h"
#include "Tile.h"
#include "RogueCMI.h"

#include <queue>
#include <map>
#include <cassert>
#include <optional>
#include <algorithm>
#include <gf/Geometry.h>
#include <gf/TileLayer.h>
#include <random>
#include <iostream>
#include <gf/Particles.h>

namespace rCMI
{

  Map::Map(RogueCMI *game)
      : m_game(game),
        map({1, 1})
  {
  }

  void Map::update_tile_at(gf::Vector2i pos, TileType type)
  {
    if (pos.x < 0 || pos.y < 0 || pos.x >= size.x || pos.y >= size.y)
      return;

    if (type == TileType::Floor)
      map.setEmpty(pos);

    if (type == TileType::Stairs)
    {
      map.setEmpty(pos);
      stairs = pos;
    }
  }

  void Map::generate_board()
  {
    size = TestMapSize;
    map = gf::SquareMap(size);

    for (int y = 0; y < size.y; ++y)
      for (int x = 0; x < size.x; ++x)
        if (x != 0 && y != 0 && x != size.x - 1 && y != size.y - 1)
          x == 1 && y == 1 ? update_tile_at({x, y}, TileType::Stairs) : update_tile_at({x, y}, TileType::Floor);
  }

  bool Map::blocking_entity_at(gf::Vector2i target)
  {
    if (target.x < 0 || target.y < 0 || target.x >= size.x || target.y >= size.y)
      return true;

    if (!map.isWalkable(target))
      return true;

    return false;
  }

  bool Map::isWalkable(gf::Vector2i position) const
  {
    if (position.x < 0 || position.y < 0 || position.x >= size.x || position.y >= size.y)
      return false;

    return map.isWalkable(position);
  }

  bool Map::isStairs(gf::Vector2i position) const
  {
    if (position.x < 0 || position.y < 0 || position.x >= size.x || position.y >= size.y)
      return false;

    return stairs == position;
  }

  void Map::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    gf::ShapeParticles particles;

    for (auto pos : map.getRange())
    {
      if (map.isInFieldOfVision(pos))
      {
        gf::Sprite sprite;
        map.isWalkable(pos)
            ? (pos == stairs
                   ? sprite.setTexture(m_game->resources.getTexture("Stairs.png"))
                   : sprite.setTexture(m_game->resources.getTexture("Floor.png")))
            : sprite.setTexture(m_game->resources.getTexture("Wall.png"));
        sprite.setPosition({(float)(pos.x * TileSize), (float)(pos.y * TileSize)});
        target.draw(sprite, states);
      }
      else if (map.isExplored(pos))
      {
        particles.addRectangle(pos * TileSize, {TileSize, TileSize},
                               map.isWalkable(pos)
                                   ? (pos == stairs ? gf::Color::Blue : gf::Color::Gray())
                                   : gf::Color::Orange);
      }
    }
    target.draw(particles);
  }

  void Map::generate_dungeon(gf::Vector2i Map_size)
  {
    size = Map_size;
    map = gf::SquareMap(size);
  }
}