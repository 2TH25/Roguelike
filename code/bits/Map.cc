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

    if (type == TileType::HealingFloor)
    {
      map.setEmpty(pos);
      healingZone.push_back(posWhBool::getPosWhBool(pos, true));
    }
  }

  void Map::generate_board()
  {
    size = TestMapSize;
    map = gf::SquareMap(size);

    for (int y = 0; y < size.y; ++y)
      for (int x = 0; x < size.x; ++x)
        if (x != 0 && y != 0 && x != size.x - 1 && y != size.y - 1)
          x == 1 && y == 1 ? update_tile_at({x, y}, TileType::HealingFloor) : update_tile_at({x, y}, TileType::Floor);


    m_game->m_WorldScene.m_world_entity.m_chestManager.spawnChest({8,7}, m_game);

    
    
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

  bool Map::usHealing(gf::Vector2i position)
  {
    auto it = std::find_if(healingZone.begin(), healingZone.end(), [&](posWhBool healPad)
                           { return position == healPad.position; });

    if (it == healingZone.end())
      return false;

    (*it).isActive = false;
    return !(*it).isActive;
  }

  bool Map::isHealing(gf::Vector2i position) const
  {
    if (position.x < 0 || position.y < 0 || position.x >= size.x || position.y >= size.y)
      return false;

    auto it = std::find_if(healingZone.begin(), healingZone.end(), [&](posWhBool healPad)
                           { return position == healPad.position; });

    return it != healingZone.end() && (*it).isActive;
  }

  void Map::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {

    gf::Texture &tileTexture = m_game->resources.getTexture("SetTileTextureSol.png");
    gf::Vector2f texture_size = tileTexture.getSize();
    gf::Vector2f tile_texture_size = texture_size / texture_size / 5;

    gf::SpriteParticles sParticle;
    sParticle.setTexture(tileTexture);
    sParticle.setScale(TileSize / (texture_size / 5.0));

    for (auto pos : map.getRange())
    {
      bool inFOV = map.isInFieldOfVision(pos);
      bool explored = map.isExplored(pos);

      // Si la tuile n'est pas explorée, on passe à la suivante (Noir total)
      if (!explored)
        continue;

      gf::Vector2i texturePosition;

      // --- 1. Détermination de la texture (identique à avant) ---
      if (isStairs(pos))
      {
        texturePosition = {0, 2};
      }
      else
      {
        int hash = (pos.x * 39113 + pos.y * 17569) % 100;
        int base = map.isWalkable(pos) ? 0 : 1;

        if (hash < 45)
        {
          int variant = (hash % 4) + 1;
          texturePosition = {variant, base};
        }
        else
        {
          texturePosition = {0, base};
        }
      }

      gf::Vector2f calculate_tile_position = ((pos * TileSize) + TileSize / 2) / sParticle.getScale();
      gf::Vector2f calculate_texture_position = texturePosition * tile_texture_size;

      sParticle.addSprite(calculate_tile_position,
                          gf::RectF::fromPositionSize(calculate_texture_position, tile_texture_size),
                          inFOV ? gf::Color::White : gf::Color::fromRgba32(100, 100, 100, 255));
    }
    sParticle.draw(target, states);

    for (auto healPad : healingZone)
    {
      if (!healPad.isActive)
        continue;

      gf::Sprite sprite;

      sprite.setTexture(m_game->resources.getTexture("mort.png"));
      sprite.setPosition({(float)(healPad.position.x * TileSize), (float)(healPad.position.y * TileSize)});

      target.draw(sprite, states);
    }
  }

  void Map::generate_dungeon(gf::Vector2i Map_size)
  {
    size = Map_size;
    map = gf::SquareMap(size);
  }
}