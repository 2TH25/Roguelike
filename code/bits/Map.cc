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
#include <gf/Math.h>

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

    int hash = (pos.x * 39113 + pos.y * 17569) % 100;

    if (type == TileType::Floor)
    {
      map.setEmpty(pos);
      texturePosition[pos.x][pos.y] = {hash < 45 ? (hash % 4) + 1 : 0, 0};
    }

    if (type == TileType::Stairs)
    {
      map.setEmpty(pos);
      stairs = pos;
      texturePosition[pos.x][pos.y] = {0, 2};
    }

    if (type == TileType::Wall)
    {
      texturePosition[pos.x][pos.y] = {hash < 45 ? (hash % 4) + 1 : 0, 1};
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

    m_game->m_WorldScene.m_world_entity.m_chestManager.spawnChest({8, 7}, m_game);
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
    if (is_mini_map)
    {
      gf::ShapeParticles shParticle;
      
      for (auto pos : map.getRange())
      {
        if (!map.isExplored(pos))
          continue;;

        if (texturePosition.at(pos.x).at(pos.y).y == 0)
          shParticle.addRectangle((pos * TileSize), {TileSize, TileSize}, gf::Color::fromRgba32(58, 48, 47));
      }

      shParticle.draw(target, states);
      return;
    }

    float nbTilesFLine = 5; // Number of tiles for one line and one column in the texture
    gf::Texture &tileTexture = m_game->resources.getTexture("SetTileTextureSol.png");
    gf::Vector2f texture_size = tileTexture.getSize();                           // Save of the texture size for calculate positions and scaling
    gf::Vector2f tile_texture_size = texture_size / texture_size / nbTilesFLine; // texture_size / texture_size to do {1, 1} / nbTilesFLine

    gf::SpriteParticles spParticle; // SpriteParticles to print all tiles wth just one draw call
    spParticle.setTexture(tileTexture);
    spParticle.setScale(TileSize / (texture_size / nbTilesFLine)); // Calculation of the scale for print tiles at the good size (tiles size on the map / tile size in the texture)

    for (auto pos : map.getRange())
    {
      bool inFOV = map.isInFieldOfVision(pos);
      bool explored = map.isExplored(pos);

      // Si la tuile n'est pas explorée, on passe à la suivante (Noir total)
      if (!explored)
        continue;

      gf::Vector2f calculate_tile_position = ((pos * TileSize) + TileSize / 2) / spParticle.getScale(); // Position of the tile on the map : ((matrix position * tile size) + midle of a tile) * (1 / Sprite scaling)
      gf::Vector2f calculate_texture_position = texturePosition.at(pos.x).at(pos.y) * tile_texture_size;                   // Position of the good part of the texture for this tile

      spParticle.addSprite(calculate_tile_position,
                          gf::RectF::fromPositionSize(calculate_texture_position, tile_texture_size),
                          inFOV ? gf::Color::White : gf::Color::fromRgba32(100, 100, 100, 255)); // for the war fog
    }
    spParticle.draw(target, states);

    for (auto healPad : healingZone)
    {
      if (!healPad.isActive || !map.isExplored(healPad.position))
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
    healingZone.clear();
  }

  void Map::setTileTexture(gf::Vector2i pos, gf::Vector2i textureCoords)
  {
    if (pos.x < 0 || pos.y < 0 || pos.x >= size.x || pos.y >= size.y)
      return;
      
    texturePosition[pos.x][pos.y] = textureCoords;
  }

  void Map::activateMiniMap()
  {
    is_mini_map = !is_mini_map;
  }
}