#include "WorldEntity.h"

#include "RogueCMI.h"
#include "Map.h"
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

  WorldEntity::WorldEntity(RogueCMI *game)
      : gf::Entity(0),
        m_game(game),
        m_map(game)
  {
  }

  void WorldEntity::update_tile_at(gf::Vector2i pos) { m_map.update_tile_at(pos); }

  void WorldEntity::generate_board()
  {
    m_map.generate_board();

    characters.clear();
    Character hero = Character::hero({5, 5}, m_game->resources.getTexture("perso640/Perso640.png"));
    const gf::Texture &textureMort = m_game->resources.getTexture("mort.png");
    hero.setDeadTexture(textureMort);
    characters.push_back(hero);
    // characters.push_back(Character::slime({5, 4}, m_game->resources.getTexture("slime.png")));
    // characters.push_back(Character::zombie({2,6}, m_game->resources.getTexture("squelette.png")));
    characters.push_back(Character::skeleton({1, 8}, m_game->resources.getTexture("squelette.png")));
  }

  std::optional<std::size_t> WorldEntity::target_character_at(gf::Vector2i target)
  {
    for (std::size_t i = 0; i < characters.size(); ++i)
      if (characters[i].getExistence().getPosition() == target && characters[i].alive())
        return i;

    return std::nullopt;
  }

  bool WorldEntity::blocking_entity_at(gf::Vector2i target) { return m_map.blocking_entity_at(target) ? true : target_character_at(target).has_value(); }

  bool WorldEntity::isWalkable(gf::Vector2i position) const { return m_map.isWalkable(position); }

  void WorldEntity::nextLevel()
  {
    generate_dungeon(m_map.getSize() * 1.2);
    std::cout << "Niveau suivant atteint !" << std::endl;
  }

  bool WorldEntity::isStairs(gf::Vector2i position) const { return m_map.isStairs(position); }

  void WorldEntity::EnemyTurns()
  {
    for (std::size_t i = 1; i < characters.size(); ++i)
      characters[i].doMove(*this);
  }

  void WorldEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    m_map.render(target, states);
    for (std::size_t i = 0; i < characters.size(); ++i)
      if ((characters[i].alive() || i == 0) && m_map.isInFieldOfVision(characters[i].getExistence().getPosition()))
        characters[i].render(target, states);
  }

  void WorldEntity::generate_dungeon(gf::Vector2i Map_size)
  {
    m_map.generate_dungeon(Map_size);

    rCMI::BSP generator;
    Dungeon dungeon = generator.generate(m_game->random, Map_size);

    for (int y = 0; y < Map_size.y; ++y)
      for (int x = 0; x < Map_size.x; ++x)
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

        Character hero = Character::hero(center, m_game->resources.getTexture("perso640/Perso640.png"));
        const gf::Texture &textureMort = m_game->resources.getTexture("mort.png");
        hero.setDeadTexture(textureMort);
        characters.push_back(hero);
        break;
      }
    }

    // if (characters.empty()) {
    //    characters.push_back(Character::hero({1,1}, m_game->resources.getTexture("perso640/Perso640.png")));
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

  std::vector<gf::Vector2i> WorldEntity::compute_path(gf::Vector2i origin, gf::Vector2i target)
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

  void WorldEntity::clearMap() { m_map.clearFieldOfVision(); }

  void WorldEntity::fieldOfVision() { m_map.computeFieldOfVision(hero().getExistence().getPosition()); }
}