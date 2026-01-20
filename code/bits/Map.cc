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
    float scaleFactor = (float)TileSize / 640.0f;

    for (auto pos : map.getRange())
    {
      bool inFOV = map.isInFieldOfVision(pos);
      bool explored = map.isExplored(pos);

      // Si la tuile n'est pas explorée, on passe à la suivante (Noir total)
      if (!explored) {
          continue;
      }

      gf::Sprite sprite;
      std::string textureName;

      // --- 1. Détermination de la texture (identique à avant) ---
      if (isStairs(pos)) {
          textureName = "Stairs.png";
      } else {
          int hash = (pos.x * 39113 + pos.y * 17569) % 100; 
          bool isWalkable = map.isWalkable(pos);
          std::string base = isWalkable ? "Floor" : "Wall";

          if (hash < 45) {
              int variant = (hash % 4) + 2; 
              textureName = base + std::to_string(variant) + ".png";
          } else {
              textureName = base + ".png";
          }
      }

      // --- 2. Configuration du Sprite ---
      sprite.setTexture(m_game->resources.getTexture(textureName));
      sprite.setPosition({(float)(pos.x * TileSize), (float)(pos.y * TileSize)});
      sprite.setScale({scaleFactor, scaleFactor});

      // --- 3. Filtre de luminosité ---
      if (inFOV) {
          // Pleine lumière
          sprite.setColor(gf::Color::White); 
      } else {
          // Brouillard de guerre : on fonce l'image (R, G, B très bas)
          // Tu peux ajuster 60, 60, 60 pour faire plus ou moins sombre
          sprite.setColor(gf::Color::fromRgba32(100, 100, 100, 255));
      }

      target.draw(sprite, states);
    }
  }
  void Map::generate_dungeon(gf::Vector2i Map_size)
  {
    size = Map_size;
    map = gf::SquareMap(size);
  }
}