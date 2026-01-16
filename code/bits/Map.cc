#include "Map.h"
#include "Tile.h"
#include "RogueCMI.h"
#include "DungeonGenerator.h"
#include "bsp.h"

#include <queue>
#include <map>
#include <cassert>
#include <optional>
#include <algorithm>
#include <gf/Geometry.h>
#include <gf/TileLayer.h>
#include <random>
#include <iostream>

namespace rCMI
{

  Map::Map(RogueCMI *game)
      : gf::Entity(0),
        m_game(game),
        tileLayer(),
        tilesetId(tileLayer.createTilesetId())
  {
  }

  void Map::update_tile_at(gf::Vector2i pos, TileType type)
  {
    if (pos.x < 0 || pos.y < 0 || pos.x >= size.x || pos.y >= size.y)
      return;
    grid[pos.y * size.x + pos.x] = type;
    int tileIndex = (type == TileType::Wall) ? 1 : 0;
    tileLayer.setTile(pos, tilesetId, tileIndex);
  }

  void Map::generate_board()
  {
    size = TestMapSize;
    grid.assign(size.x * size.y, TileType::Floor);
    tileLayer = gf::TileLayer::createOrthogonal(size, {80, 80});
    tilesetId = tileLayer.createTilesetId();
    gf::Tileset &ts = tileLayer.getTileset(tilesetId);
    ts.setTexture(m_game->resources.getTexture("tileSetTexture.png"));
    ts.setTileSize({80, 80});

    for (int y = 0; y < size.y; ++y)
    {
      for (int x = 0; x < size.x; ++x)
      {
        gf::Vector2i pos = {x, y};
        TileType type = (x == 0 || y == 0 || x == size.x - 1 || y == size.y - 1)
                            ? TileType::Wall
                            : TileType::Floor;
        update_tile_at(pos, type);
      }
    }

    characters.clear();
    Character hero = Character::hero({5, 5}, m_game->resources.getTexture("perso70.png"));
    const gf::Texture &textureMort = m_game->resources.getTexture("mort.png");
    hero.setDeadTexture(textureMort);
    characters.push_back(hero);
    // characters.push_back(Character::slime({5, 4}, m_game->resources.getTexture("slime.png")));
    // characters.push_back(Character::zombie({2,6}, m_game->resources.getTexture("squelette.png")));
    characters.push_back(Character::skeleton({1, 8}, m_game->resources.getTexture("squelette.png")));
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
    if (target.x < 0 || target.y < 0 || target.x >= size.x || target.y >= size.y)
    {
      return true;
    }

    if (grid[target.y * size.x + target.x] == TileType::Wall)
    {
      return true;
    }

    return target_character_at(target).has_value();
  }

  bool Map::isWalkable(gf::Vector2i position) const
  {
    if (position.x < 0 || position.y < 0 || position.x >= size.x || position.y >= size.y)
    {
      return false;
    }
    return grid[position.y * size.x + position.x] == TileType::Floor;
  }

  void Map::EnemyTurns()
  {
    for (std::size_t i = 1; i < characters.size(); ++i)
    {
      characters[i].doMove(*this);
    }
  }

  void Map::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    target.draw(tileLayer, states);

    for (std::size_t i = 0; i < characters.size(); ++i)
    {
      if (characters[i].alive() || i == 0)
      {
        characters[i].render(target, states);
      }
    }
  }

  void Map::generate_dungeon(gf::Vector2i Map_size)
  {
    size = Map_size;
    tileLayer = gf::TileLayer::createOrthogonal(size, {80, 80});
    tilesetId = tileLayer.createTilesetId();
    gf::Tileset &ts = tileLayer.getTileset(tilesetId);
    ts.setTexture(m_game->resources.getTexture("tileSetTexture.png"));
    ts.setTileSize({80, 80});

    rCMI::BSP generator;

    Dungeon dungeon = generator.generate(m_game->random);

    grid.assign(size.x * size.y, TileType::Wall); // On remplit de murs par défaut

    for (int y = 0; y < size.y; ++y)
    {
      for (int x = 0; x < size.x; ++x)
      {
        gf::Vector2i pos = {x, y};

        TileType type = dungeon.getTile({x, y});

        if (type == TileType::Wall)
        {

          bool touchesFloor = false;
          for (int dy = -1; dy <= 1; ++dy)
          {
            for (int dx = -1; dx <= 1; ++dx)
            {
              if (dx == 0 && dy == 0)
                continue;
              if (dungeon.getTile(pos + gf::Vector2i{dx, dy}) == TileType::Floor)
              {
                touchesFloor = true;
                break;
              }
            }
            if (touchesFloor)
            {
              break;
            }
          }

          if (touchesFloor)
          {
            update_tile_at(pos, TileType::Wall);
          }
          else
          {
            grid[y * size.x + x] = TileType::Wall;
            tileLayer.setTile(pos, tilesetId, -1); // -1 pour cacher la tuile
          }
        }
        else
        {
          update_tile_at(pos, TileType::Floor);
        }
      }
    }

    characters.clear();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist_x(0, size.x - 1);
    std::uniform_int_distribution<> dist_y(0, size.y - 1);
    std::uniform_int_distribution<> dist_type(1, 100);

    gf::Vector2i pos_aleatoire_hero;

    do
    {
      pos_aleatoire_hero.x = dist_x(gen);
      pos_aleatoire_hero.y = dist_y(gen);
    } while (!isWalkable(pos_aleatoire_hero));

    Character hero = Character::hero({pos_aleatoire_hero.x, pos_aleatoire_hero.y}, m_game->resources.getTexture("perso70.png"));
    const gf::Texture &textureMort = m_game->resources.getTexture("mort.png");
    hero.setDeadTexture(textureMort);
    characters.push_back(hero);

    for (int i = 0; i < MaxMonstersMin; i++)
    {
      gf::Vector2i pos_aleatoire;

      do
      {
        pos_aleatoire.x = dist_x(gen);
        pos_aleatoire.y = dist_y(gen);
      } while (!isWalkable(pos_aleatoire));

      int nombre_aleatoire = dist_type(gen);
      if (nombre_aleatoire <= 60)
      { // 60% de chance
        characters.push_back(Character::slime({pos_aleatoire.x, pos_aleatoire.y}, m_game->resources.getTexture("slime.png")));
      }
      else if (nombre_aleatoire <= 85)
      { // 25% de chance
        characters.push_back(Character::zombie({pos_aleatoire.x, pos_aleatoire.y}, m_game->resources.getTexture("zombie.png")));
      }
      else
      { // 15% de chance
        characters.push_back(Character::skeleton({pos_aleatoire.x, pos_aleatoire.y}, m_game->resources.getTexture("squelette.png")));
      }
    }
  }

  struct VectorCompare
  {
    bool operator()(const gf::Vector2i &a, const gf::Vector2i &b) const
    {
      if (a.x != b.x)
        return a.x < b.x;
      return a.y < b.y;
    }
  };

  std::vector<gf::Vector2i> Map::compute_path(gf::Vector2i origin, gf::Vector2i target)
  {
    std::queue<gf::Vector2i> frontier;
    frontier.push(origin);

    std::map<gf::Vector2i, gf::Vector2i, VectorCompare> came_from;
    came_from[origin] = origin;
    gf::Vector2i directions[] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    while (!frontier.empty())
    {
      gf::Vector2i current = frontier.front();
      frontier.pop();
      if (current == target)
      {
        break;
      }

      for (auto &dir : directions)
      {
        gf::Vector2i next = current + dir;

        if (came_from.find(next) == came_from.end())
        {

          if (isWalkable(next) || next == target)
          {
            frontier.push(next);
            came_from[next] = current;
          }
        }
      }
    }
    std::vector<gf::Vector2i> path;

    if (came_from.find(target) == came_from.end())
    {
      return path;
    }

    gf::Vector2i current = target;
    while (current != origin)
    {
      path.push_back(current);
      current = came_from[current];
    }
    std::reverse(path.begin(), path.end());

    return path;
  }

}