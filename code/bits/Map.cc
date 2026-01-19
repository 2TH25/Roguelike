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
#include <gf/Particles.h>

namespace rCMI
{

  Map::Map(RogueCMI *game)
      : gf::Entity(0),
        m_game(game)
  {
  }

  void Map::update_tile_at(gf::Vector2i pos)
  {
    if (pos.x < 0 || pos.y < 0 || pos.x >= size.x || pos.y >= size.y)
      return;

    grid->setEmpty(pos);
  }

  void Map::generate_board()
  {
    if (grid != nullptr)
      free(grid);

    size = TestMapSize;
    grid = new gf::SquareMap(size);

    for (int y = 0; y < size.y; ++y)
      for (int x = 0; x < size.x; ++x)
        if (x != 0 && y != 0 && x != size.x - 1 && y != size.y - 1)
          update_tile_at({x, y});

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
      if (characters[i].getExistence().getPosition() == target && characters[i].alive())
        return i;

    return std::nullopt;
  }

  bool Map::blocking_entity_at(gf::Vector2i target)
  {
    if (target.x < 0 || target.y < 0 || target.x >= size.x || target.y >= size.y)
      return true;

    if (!grid->isWalkable(target))
      return true;

    return target_character_at(target).has_value();
  }

  bool Map::isWalkable(gf::Vector2i position) const
  {
    if (position.x < 0 || position.y < 0 || position.x >= size.x || position.y >= size.y)
      return false;

    return grid->isWalkable(position);
  }

  void Map::nextLevel()
  {
    generate_dungeon(this->size * 1.2);
    std::cout << "Niveau suivant atteint !" << std::endl;
  }

  bool Map::isStairs(gf::Vector2i position) const
  {
    if (position.x < 0 || position.y < 0 || position.x >= size.x || position.y >= size.y)
      return false;
    // return grid[position.y * size.x + position.x] == TileType::Stairs;
    return false;
  }

  void Map::EnemyTurns()
  {
    for (std::size_t i = 1; i < characters.size(); ++i)
      characters[i].doMove(*this);
  }

  void Map::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    gf::ShapeParticles particles;

    for (auto pos : grid->getRange())
    {
      if (grid->isInFieldOfVision(pos))
      {
        gf::Sprite sprite;
        grid->isWalkable(pos) ? sprite.setTexture(m_game->resources.getTexture("Floor.png")) : sprite.setTexture(m_game->resources.getTexture("Wall.png"));
        sprite.setPosition({(float)(pos.x * TileSize), (float)(pos.y * TileSize)});
        target.draw(sprite, states);
      }
      else if (grid->isExplored(pos))
      {
        particles.addRectangle(pos * TileSize, {TileSize, TileSize},
                               grid->isWalkable(pos) ? gf::Color::Gray() : gf::Color::Orange);
      }
    }

    target.draw(particles);

    for (std::size_t i = 0; i < characters.size(); ++i)
      if ((characters[i].alive() || i == 0) && grid->isInFieldOfVision(characters[i].getExistence().getPosition()))
        characters[i].render(target, states);
  }

  void Map::generate_dungeon(gf::Vector2i Map_size)
  {
    if (grid != nullptr)
      free(grid);

    size = Map_size;
    grid = new gf::SquareMap(size);

    rCMI::BSP generator;
    Dungeon dungeon = generator.generate(m_game->random, size);

    for (int y = 0; y < size.y; ++y)
      for (int x = 0; x < size.x; ++x)
      {
        TileType type = dungeon.getTile({x, y});
        if (type == TileType::Floor)
          update_tile_at({x, y});
      }

    characters.clear();
    std::vector<BSPTree *> leaves;
    generator.getRoot().getRooms(leaves);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist_type(1, 100);

    for (auto *leaf : leaves)
    {
      if (leaf->type == RoomType::Start)
      {
        gf::Vector2i center = leaf->room.getCenter();

        Character hero = Character::hero(center, m_game->resources.getTexture("perso70.png"));
        const gf::Texture &textureMort = m_game->resources.getTexture("mort.png");
        hero.setDeadTexture(textureMort);
        characters.push_back(hero);
        break;
      }
    }

    // if (characters.empty()) {
    //    characters.push_back(Character::hero({1,1}, m_game->resources.getTexture("perso70.png")));
    // }

    for (auto *leaf : leaves)
    {
      gf::RectI room = leaf->room;
      gf::Vector2i center = room.getCenter();

      if (leaf->type == RoomType::End)
      {
        // update_tile_at(center, TileType::Stairs);
      }
      else if (leaf->type == RoomType::Healing)
      {
        // update_tile_at(center, TileType::HealingFloor);
        continue;
      }
      else if (leaf->type == RoomType::Start)
      {
        continue;
      }
      else
      {
        std::uniform_int_distribution<> dist_nb_mob(0, 2);
        int nbMobs = dist_nb_mob(gen);

        std::uniform_int_distribution<> dist_room_x(room.min.x + 1, room.max.x - 1);
        std::uniform_int_distribution<> dist_room_y(room.min.y + 1, room.max.y - 1);

        for (int i = 0; i < nbMobs; i++)
        {
          gf::Vector2i pos_aleatoire;
          bool validPos = false;
          int attempts = 0;

          do
          {
            pos_aleatoire.x = dist_room_x(gen);
            pos_aleatoire.y = dist_room_y(gen);
            if (isWalkable(pos_aleatoire) && !target_character_at(pos_aleatoire).has_value())
            {
              validPos = true;
            }
            attempts++;
          } while (!validPos && attempts < 10);

          if (validPos)
          {
            Character mob;
            int nombre_aleatoire = dist_type(gen);

            if (nombre_aleatoire <= 60)
            {
              mob = Character::slime(pos_aleatoire, m_game->resources.getTexture("slime.png"));
            }
            else if (nombre_aleatoire <= 85)
            {
              mob = Character::zombie(pos_aleatoire, m_game->resources.getTexture("zombie.png"));
            }
            else
            {
              mob = Character::skeleton(pos_aleatoire, m_game->resources.getTexture("squelette.png"));
            }
            mob.setHomeRoom(room);
            characters.push_back(mob);
          }
        }
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

  void Map::clearMap()
  {
    grid->clearFieldOfVision();
  }

  void Map::fieldOfVision()
  {
    grid->computeFieldOfVision(hero().getExistence().getPosition(), 5);
  }
}