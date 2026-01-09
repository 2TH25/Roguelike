#include "Map.h"
#include "Tile.h"
#include "RogueCMI.h"
#include "DungeonGenerator.h"

#include <cassert>
#include <optional>
#include <algorithm>
#include <gf/Geometry.h>
#include <gf/TileLayer.h>

namespace rCMI
{

  Map::Map(RogueCMI *game)
  : gf::Entity(0)
  , m_game(game)
  , tileLayer()
  {
    tilesetId = tileLayer.createTilesetId();
    gf::Tileset& ts = tileLayer.getTileset(tilesetId);
    ts.setTexture(m_game->resources.getTexture("tileSetTexture.jpg"));
    ts.setTileSize({80, 80});

    generate_board(game);
  }

  void Map::update_tile_at(gf::Vector2i pos, TileType type)
  {
    grid[pos.y * MapSize.x + pos.x] = type;
    int tileIndex = (type == TileType::Wall) ? 1 : 0;
    tileLayer.setTile(pos, tilesetId, tileIndex);
  }

  void Map::generate_board(RogueCMI *m_game)
  {
    grid.assign(MapSize.x * MapSize.y, TileType::Floor);
    tileLayer = gf::TileLayer::createOrthogonal(MapSize, {80, 80});
    tilesetId = tileLayer.createTilesetId();
    gf::Tileset &ts = tileLayer.getTileset(tilesetId);
    ts.setTexture(m_game->resources.getTexture("tileSetTexture.jpg"));
    ts.setTileSize({80, 80});

    for (int y = 0; y < MapSize.y; ++y)
    {
      for (int x = 0; x < MapSize.x; ++x)
      {
        gf::Vector2i pos = {x, y};
        TileType type = (x == 0 || y == 0 || x == MapSize.x - 1 || y == MapSize.y - 1)
                            ? TileType::Wall
                            : TileType::Floor;
        update_tile_at(pos, type);
      }
    }
  }

  std::optional<std::size_t> Map::target_character_at(gf::Vector2i target)
  {
    for (std::size_t i = 0; i < characters.size(); ++i)
    {
      if (characters[i].getExistence().getPosition() == target && characters[i].alive())
      {
        return i;
      }
    }
    return std::nullopt;
  }

  bool Map::blocking_entity_at(gf::Vector2i target)
  {
    if (target.x < 0 || target.y < 0 || target.x >= MapSize.x || target.y >= MapSize.y)
    {
      return true;
    }
    
    if (grid[target.y * MapSize.x + target.x] == TileType::Wall)
    {
      return true;
    }

    return target_character_at(target).has_value();
  }

  bool Map::isWalkable(gf::Vector2i position) const
  {
    if (position.x < 0 || position.y < 0 || position.x >= MapSize.x || position.y >= MapSize.y)
    {
      return false;
    }
    return grid[position.y * MapSize.x + position.x] == TileType::Floor;
  }

  void Map::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    target.draw(tileLayer, states);

    for (auto &character : characters)
    {
      character.render(target, states);
    }
  }
}
